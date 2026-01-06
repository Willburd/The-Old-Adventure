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
            // Preprocessing and room ticks
            /////////////////////////////////////////////////
            OnPreGameTick();
            List<Entity> active_entities = [];
            List<Room> initing_rooms = [];
            foreach(Entity ent in Entity.EntityList)
            {
                if(!ent.IsInitilized) 
                {
                    ent.OnInit(); // Actually setup entites, needed for create and asset loading signals.
                    if(ent.GetType() == typeof(Room)) initing_rooms.Add((Room)ent);
                }
                // Preupdate
                if(!EditorMode || EditorAllowsUpdates) 
                {
                    ent.SendSignal(Signals.pre_update, ent.Enabled);
                }
                // Handle movement interpolation
                if(ent.Enabled) 
                {
                    ent.SnapTransform(); // Update the previous location transform
                    active_entities.Add(ent);
                }
            }

            // Editor update
            if(EditorMode) 
            {
                foreach(Entity ent in active_entities)
                {
                    ent.SendSignal(Signals.editor_update);
                }
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
                    if(room.Enabled) 
                    {
                        room.OnRoomUpdate();
                    }
                    else
                    {
                        room.OnRoomDisabledUpdate();
                    }
                }
                
                /////////////////////////////////////////////////
                // Physics and Collisions
                /////////////////////////////////////////////////
                OnPhysicsTick();
                foreach(Entity ent in active_entities)
                {
                    ent.SendSignal(Signals.apply_physics);
                }
                // Reset collisions and triggers list for global signal
                Collider.Collision.all_collisions.Clear();
                Collider.Collision.all_triggered.Clear(); 
                // Time to resolve all collisions! Check every collider with every OTHER collider... This is pretty expensive!
                List<EntComponent> all_colliders = [];
                all_colliders.AddRange(EntComponent.GetAllOfType(typeof(Collider)));
                all_colliders.AddRange(EntComponent.GetAllOfType(typeof(TriggerVolume)));
                foreach(Collider collider in all_colliders.Cast<Collider>())
                {
                    if(!collider.Host.IsInitilized || !collider.Host.Enabled || !collider.Active) continue;
                    collider.CheckCollisions(all_colliders);
                }
                // Global signals for collisions and triggers
                if(Collider.Collision.all_collisions.Count > 0) Entity.SendGlobalSignal(Signals.global_all_collisions, Collider.Collision.all_collisions);
                if(Collider.Collision.all_triggered.Count > 0) Entity.SendGlobalSignal(Signals.global_all_triggers, Collider.Collision.all_triggered);

                /////////////////////////////////////////////////
                // Processing
                /////////////////////////////////////////////////
                OnGameTick();
                foreach(Entity ent in active_entities)
                {
                    ent.SendSignal(Signals.update);
                }
                OnPostGameTick();
                foreach(Entity ent in active_entities)
                {
                    ent.SendSignal(Signals.post_update);
                }
            }

            // Finish up by telling the next frame if we held the key or not
            input_handler.InputStateUpdate();
        }
    }
}