using System.Numerics;
using EntComponents;

namespace Engine
{
    public partial class Core
    {
        /// <summary>
        /// Number of game ticks per second for gameplay updates.
        /// </summary>
        protected static double TICKRATE {get; set;} = 40;

        /// <summary>
        /// The threshold needed for the delta_time accumulator to trigger a gametick.
        /// </summary>
        private static double GameTickInterval {get{ return 1.0 / TICKRATE; }}
        private static double game_tick_accumulator = 0;

        /// <summary>
        /// Skips delta_time check for updating.
        /// </summary>
        private static bool RequestUpdate {get; set;}

        /// <summary>
        /// Number of game ticks since launch.
        /// </summary>
        public static long ElapsedGameTicks {get; private set;}


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Gameloop processing
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Handles calling GameTicks at the desired interval. Called by the window itself.
        /// </summary>
        private static void HandleWindowUpdate(double deltaTime)
        {
            game_tick_accumulator += deltaTime;
            if(game_tick_accumulator >= GameTickInterval || RequestUpdate)
            {
                ElapsedGameTicks++;
                singleton?.GameTick();
                game_tick_accumulator %= GameTickInterval;
                RequestUpdate = false;
            }
        }

        /// <summary>
        /// Game tick fired. This processes all game objects, and any special logic during them.
        /// </summary>
        private void GameTick()
        {
            /////////////////////////////////////////////////
            // Threading batch control
            /////////////////////////////////////////////////
            
            int batch_size = 512;
            ThreadPool.GetMaxThreads(out int total_available_threads, out int max_asyncthread_count);
            batch_size = Math.Min(batch_size, max_asyncthread_count - 1); // incase we're running on something with more limited threads
            // We never want to overrun our task pool, otherwise we'll hit the dreaded 0.5 second reschedual in a gametick.

            List<Task> thread_batch = new List<Task>();
            Action<List<Task>> AwaitCurrentBatch = thread_batch =>
            {
                if(thread_batch.Count == 0) return;
                Task.WaitAll(thread_batch);
                thread_batch.Clear();
            };

            /////////////////////////////////////////////////
            // Entity creation
            /////////////////////////////////////////////////
            OnPreGameTick();
            Entity.ActiveEntities.Clear();
            List<Room> initing_rooms = [];
            // Cannot thread this due to asset loading file handle maximums
            foreach(Entity ent in Entity.UninitEntityList)
            {
                ent.OnInit(); // Actually setup entites, needed for create and asset loading signals.
                if(ent.GetType() == typeof(Room)) initing_rooms.Add((Room)ent);
                Entity.EntityList.Add(ent);
            }
            Entity.UninitEntityList.Clear();

            /////////////////////////////////////////////////
            // Preprocessing
            /////////////////////////////////////////////////
            foreach(Entity ent in Entity.EntityList)
            {
                thread_batch.Add(Task.Run(() =>
                {
                    // Preupdate
                    if(!EditorMode || EditorAllowsUpdates) 
                    {
                        ent.SendSignal(Signals.pre_update, ent.Enabled);
                    }
                    // Handle movement interpolation
                    if(ent.Enabled) 
                    {
                        ent.SnapTransform(); // Update the previous location transform
                        Entity.ActiveEntities.Add(ent);
                    }
                }));
                if(thread_batch.Count >= batch_size) AwaitCurrentBatch(thread_batch);
            }
            AwaitCurrentBatch(thread_batch);

            /////////////////////////////////////////////////
            // Editor update
            /////////////////////////////////////////////////
            if(EditorMode) 
            {
                foreach(Entity ent in Entity.ActiveEntities)
                {
                    thread_batch.Add(Task.Run(() =>
                    {
                        ent.SendSignal(Signals.editor_update);
                    }));
                    if(thread_batch.Count >= batch_size) AwaitCurrentBatch(thread_batch);
                }
                AwaitCurrentBatch(thread_batch);
            }

            // There are no gameticks during editor mode unless we unpause it
            if(!EditorMode || EditorAllowsUpdates) 
            {
                /////////////////////////////////////////////////
                // Handle room processing
                /////////////////////////////////////////////////
                // We do room loaded signal AFTER everything else is init, or we'll miss some!
                foreach(Room room in initing_rooms)
                {
                    Entity.SendGlobalSignal(Signals.global_room_loaded, room);
                }

                // Handle room ticks in a special way to keep sane order
                List<Room> processing_rooms = [.. Room.loaded_rooms];
                foreach(Room room in processing_rooms)
                {
                    thread_batch.Add(Task.Run(() =>
                    {
                        if(room.Enabled) 
                        {
                            room.OnRoomUpdate();
                        }
                        else
                        {
                            room.OnRoomDisabledUpdate();
                        }
                    }));
                    if(thread_batch.Count >= batch_size) AwaitCurrentBatch(thread_batch);
                }
                AwaitCurrentBatch(thread_batch);
                
                /////////////////////////////////////////////////
                // Physics and Collisions
                /////////////////////////////////////////////////
                OnPhysicsTick();
                foreach(Entity ent in Entity.ActiveEntities)
                {
                    thread_batch.Add(Task.Run(() =>
                    {
                        ent.SendSignal(Signals.apply_physics);
                    }));
                    if(thread_batch.Count >= batch_size) AwaitCurrentBatch(thread_batch);
                }
                AwaitCurrentBatch(thread_batch);

                // Reset collisions and triggers list for global signal
                Collider.Collision.all_collisions.Clear();
                Collider.Collision.all_triggered.Clear(); 
                // Time to resolve all collisions! Check every collider with every OTHER collider... This is pretty expensive!
                List<EntComponent> all_colliders = [];
                all_colliders.AddRange(EntComponent.GetAllOfType(typeof(Collider)));
                all_colliders.AddRange(EntComponent.GetAllOfType(typeof(TriggerVolume)));
                foreach(Collider collider in all_colliders.Cast<Collider>())
                {
                    thread_batch.Add(Task.Run(() =>
                    {
                        if(!collider.Host.IsInitilized || !collider.Host.Enabled || !collider.Active) return;
                        collider.CheckCollisions(all_colliders);
                    }));
                    if(thread_batch.Count >= batch_size) AwaitCurrentBatch(thread_batch);
                }
                AwaitCurrentBatch(thread_batch);

                // Global signals for collisions and triggers
                if(Collider.Collision.all_collisions.Count > 0) Entity.SendGlobalSignal(Signals.global_all_collisions, Collider.Collision.all_collisions);
                if(Collider.Collision.all_triggered.Count > 0) Entity.SendGlobalSignal(Signals.global_all_triggers, Collider.Collision.all_triggered);

                /////////////////////////////////////////////////
                // Processing
                /////////////////////////////////////////////////
                OnGameTick();
                foreach(Entity ent in Entity.ActiveEntities)
                {
                    thread_batch.Add(Task.Run(() =>
                    {
                        ent.SendSignal(Signals.update);
                    }));
                    if(thread_batch.Count >= batch_size) AwaitCurrentBatch(thread_batch);
                }
                AwaitCurrentBatch(thread_batch);

                OnPostGameTick();
                foreach(Entity ent in Entity.ActiveEntities)
                {
                    thread_batch.Add(Task.Run(() =>
                    {
                        ent.SendSignal(Signals.post_update);
                    }));
                    if(thread_batch.Count >= batch_size) AwaitCurrentBatch(thread_batch);
                }
                AwaitCurrentBatch(thread_batch);
            }
            

            /////////////////////////////////////////////////
            // Entity destruction
            /////////////////////////////////////////////////
            while(Entity.DestructingEntities.Count > 0) // If an entity destroys other entities on its destruction then we must process those too.
            {
                List<Entity> current_destructing_batch = [.. Entity.DestructingEntities];
                Entity.DestructingEntities.Clear(); // Clear out for the next batch if any subdestructions happen
                foreach(Entity ent in current_destructing_batch) // can't edit the current batch so we must repeatedly copy our list each batch
                {
                    thread_batch.Add(Task.Run(() =>
                    {
                        if(!ent.IsInitilized)
                        {
                            Entity.UninitEntityList.Remove(ent);
                        }
                        else
                        {
                            Entity.EntityList.Remove(ent);
                        }
                    }));
                    if(thread_batch.Count >= batch_size) AwaitCurrentBatch(thread_batch);
                }
                AwaitCurrentBatch(thread_batch);
            }
            
            /////////////////////////////////////////////////
            // shutdown
            /////////////////////////////////////////////////
            if(shutting_down)
            {
                WindowContext.Close();
                return;
            }

            // Finish up by telling the next frame if we held the key or not
            input_handler.InputStateUpdate();
        }
    }
}