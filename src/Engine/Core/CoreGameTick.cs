using System.Numerics;
using EntComponents;

namespace Engine
{
    public partial class Core
    {
        /// <summary>
        /// Number of game ticks per second for gameplay updates.
        /// </summary>
        protected static double TICKRATE { get; set; } = 40;

        /// <summary>
        /// The threshold needed for the delta_time accumulator to trigger a gametick.
        /// </summary>
        private static double GameTickInterval { get { return 1.0 / TICKRATE; } }
        private static double game_tick_accumulator = 0;

        /// <summary>
        /// Skips delta_time check for updating.
        /// </summary>
        private static bool RequestUpdate { get; set; }

        /// <summary>
        /// Number of game ticks since launch.
        /// </summary>
        public static long ElapsedGameTicks { get; private set; }
        
        /// <summary>
        /// Number of game ticks since launch.
        /// </summary>
        public static double RenderPreciseGameTick(double tick_delta)
        { 
            return ElapsedGameTicks + (double)tick_delta;
        }

        /// <summary>
        /// Distance from the camera that entities will be considered disabled, even if their enabled flag is true.
        /// </summary>
        public const float world_unload_radius = 100f;


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Gameloop processing
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Handles calling GameTicks at the desired interval. Called by the window itself.
        /// </summary>
        private static void HandleWindowUpdate(double deltaTime)
        {
            game_tick_accumulator += deltaTime;
            if (game_tick_accumulator >= GameTickInterval || RequestUpdate)
            {
                ElapsedGameTicks++;
                singleton.GameTick();
                game_tick_accumulator %= GameTickInterval;
                RequestUpdate = false;
            }
        }

        /// <summary>
        /// Game tick fired. This processes all game objects, and any special logic during them.
        /// </summary>
        private void GameTick()
        {
            // threading
            List<Task> thread_batch = new List<Task>();

            /////////////////////////////////////////////////
            // Entity creation
            /////////////////////////////////////////////////
            OnPreGameTick();
            List<Entity> active_entities = [];
            List<Room> initing_rooms = [];
            // Cannot thread this due to asset loading file handle maximums
            while(Entity.UninitEntityList.Count > 0)
            {
                List<Entity> uninit_batch = [.. Entity.UninitEntityList];
                Entity.UninitEntityList.Clear();
                foreach (Entity ent in uninit_batch)
                {
                    ent.OnInit(); // Actually setup entites, needed for create and asset loading signals.
                    if (ent.GetType().IsSubclassOf(typeof(Room)))
                    {
                        (ent as Room).RoomInit();
                        initing_rooms.Add((Room)ent);
                    }
                    Entity.EntityList.Add(ent);
                    Entity.EntityLookupList.Add(ent.EntityID, ent);
                }
            }

            /////////////////////////////////////////////////
            // Preprocessing
            /////////////////////////////////////////////////
            Vector3 world_load_position = Vector3.Zero;
            if (Camera.WorldCamera != null) world_load_position = Camera.WorldCamera.Position;
            foreach (Entity ent in Entity.EntityList)
            {
                thread_batch.Add(Task.Run(() =>
                {
                    if (ent == null) return; // TODO - Discover the desync
                    // Preupdate
                    if (!EditorMode || EditorAllowsUpdates)
                    {
                        ent.SendSignal(Signals.pre_update, ent.RoomEnabled());
                    }
                    // Handle movement interpolation
                    if (ent.RoomEnabled())
                    {
                        ent.SnapTransform(); // Update the previous location transform
                        if (Vector3.Distance(world_load_position, ent.Position) <= world_unload_radius) active_entities.Add(ent);
                    }
                }));
                if (thread_batch.Count >= BatchSize) AwaitCurrentBatch(thread_batch);
            }
            AwaitCurrentBatch(thread_batch);

            /////////////////////////////////////////////////
            // Editor update
            /////////////////////////////////////////////////
            if (EditorMode)
            {
                foreach (Entity ent in active_entities)
                {
                    thread_batch.Add(Task.Run(() =>
                    {
                        if (ent == null) return; // TODO - Discover the desync
                        ent.SendSignal(Signals.editor_update);
                    }));
                    if (thread_batch.Count >= BatchSize) AwaitCurrentBatch(thread_batch);
                }
                AwaitCurrentBatch(thread_batch);
            }

            // There are no gameticks during editor mode unless we unpause it
            if (!EditorMode || EditorAllowsUpdates)
            {
                /////////////////////////////////////////////////
                // Handle room processing
                /////////////////////////////////////////////////
                // We do room loaded signal AFTER everything else is init, or we'll miss some!
                foreach (Room room in initing_rooms)
                {
                    Entity.SendGlobalSignal(Signals.global_room_loaded, room);
                }

                // Handle room ticks in a special way to keep sane order
                List<Room> processing_rooms = [.. Room.loaded_rooms];
                foreach (Room room in processing_rooms)
                {
                    thread_batch.Add(Task.Run(() =>
                    {
                        if (room == null) return; // TODO - Discover the desync
                        if (room.RoomEnabled())
                        {
                            room.Environment?.Update();
                            room.TempEnvironmentOverride?.Update();
                            room.OnRoomUpdate();
                        }
                        else
                        {
                            room.OnRoomDisabledUpdate();
                        }
                    }));
                    if (thread_batch.Count >= BatchSize) AwaitCurrentBatch(thread_batch);
                }
                AwaitCurrentBatch(thread_batch);

                /////////////////////////////////////////////////
                // Physics and Collisions
                /////////////////////////////////////////////////
                OnPhysicsTick();
                foreach (Entity ent in active_entities)
                {
                    thread_batch.Add(Task.Run(() =>
                    {
                        if (ent == null) return; // TODO - Discover the desync
                        ent.SendSignal(Signals.apply_physics);
                    }));
                    if (thread_batch.Count >= BatchSize) AwaitCurrentBatch(thread_batch);
                }
                AwaitCurrentBatch(thread_batch);

                // Reset collisions and triggers list for global signal
                Collider.Collision.all_collisions.Clear();
                Collider.Collision.all_triggered.Clear();
                // Time to resolve all collisions! Check every collider with every OTHER collider... This is pretty expensive!
                List<EntComponent> all_colliders = [];
                all_colliders.AddRange(EntComponent.GetAllOfType(typeof(Collider)));
                all_colliders.AddRange(EntComponent.GetAllOfType(typeof(TriggerVolume)));
                foreach (Collider collider in all_colliders.Cast<Collider>())
                {
                    thread_batch.Add(Task.Run(() =>
                    {
                        if (collider == null) return; // TODO - Discover the desync
                        if (!collider.Host.IsInitilized || !collider.Host.RoomEnabled() || !collider.Active) return;
                        collider.CheckCollisions(all_colliders);
                    }));
                    if (thread_batch.Count >= BatchSize) AwaitCurrentBatch(thread_batch);
                }
                AwaitCurrentBatch(thread_batch);

                // Global signals for collisions and triggers
                if (Collider.Collision.all_collisions.Count > 0) Entity.SendGlobalSignal(Signals.global_all_collisions, Collider.Collision.all_collisions);
                if (Collider.Collision.all_triggered.Count > 0) Entity.SendGlobalSignal(Signals.global_all_triggers, Collider.Collision.all_triggered);

                /////////////////////////////////////////////////
                // Processing
                /////////////////////////////////////////////////
                OnGameTick();
                foreach (Entity ent in active_entities)
                {
                    thread_batch.Add(Task.Run(() =>
                    {
                        if (ent == null) return; // TODO - Discover the desync
                        ent.SendSignal(Signals.update);
                    }));
                    if (thread_batch.Count >= BatchSize) AwaitCurrentBatch(thread_batch);
                }
                AwaitCurrentBatch(thread_batch);

                OnPostGameTick();
                foreach (Entity ent in active_entities)
                {
                    thread_batch.Add(Task.Run(() =>
                    {
                        if (ent == null) return; // TODO - Discover the desync
                        ent.SendSignal(Signals.post_update);
                    }));
                    if (thread_batch.Count >= BatchSize) AwaitCurrentBatch(thread_batch);
                }
                AwaitCurrentBatch(thread_batch);
            }


            /////////////////////////////////////////////////
            // Entity destruction
            /////////////////////////////////////////////////
            // Requires the main thread for GL context to be able to dispose
            // Repeats until all entities are cleaned up, as one entity Destroy() may call another!
            while (Entity.DestructingEntities.Count > 0)
            {
                List<Entity> destroy_batch = [.. Entity.DestructingEntities];
                Entity.DestructingEntities.Clear();
                foreach (Entity ent in destroy_batch)
                {
                    // Remove from processing
                    if (!ent.IsInitilized)
                    {
                        Entity.UninitEntityList.Remove(ent);
                    }
                    else
                    {
                        Entity.EntityList.Remove(ent);
                        Entity.EntityLookupList.Remove(ent.EntityID);
                    }
                    // Remove component parts
                    foreach (EntComponent component in ent.GetAllComponents())
                    {
                        ent.RemoveComponent(component);
                    }
                    // Debug info
                    if (ent.GetType().IsSubclassOf(typeof(Actor)))
                    {
                        // Show our linked room
                        Actor us_as_actor = (Actor)ent;
                        Console.WriteLine("EntityDestroy-X (" + ent.EntityID + ")[" + us_as_actor.OwnerRoom?.EntityID + "] : " + ent.AssetKey);
                    }
                    else
                    {
                        // Just delete info
                        Console.WriteLine("EntityDestroy-X (" + ent.EntityID + ") : " + ent.AssetKey);
                    }
                    // finish up
                    ent.OnCleanup();
                }
            }

            /////////////////////////////////////////////////
            // shutdown
            /////////////////////////////////////////////////
            if (shutting_down)
            {
                WindowContext.Close();
                return;
            }

            // Finish up by telling the next frame if we held the key or not
            InputHandler.InputStateUpdate();
        }
    }
}