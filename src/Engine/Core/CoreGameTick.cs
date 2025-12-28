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
            if(game_tick_accumulator >= GameTickInterval)
            {
                ElapsedGameTicks++;
                singleton?.GameTick();
                game_tick_accumulator -= GameTickInterval;
            }
        }

        /// <summary>
        /// Game tick fired. This processes all game objects, and any special logic during them.
        /// </summary>
        private void GameTick()
        {
            // Preprocessing and room ticks
            OnPreGameTick();
            List<Entity> active_entities = [];
            foreach(Entity ent in Entity.EntityList)
            {
                if(!ent.IsInitilized) ent.OnInit(); // Actually setup entites, needed for create and asset loading signals.
                ent.SendSignal(Signals.pre_update, ent.Enabled);
                if(ent.Enabled) 
                {
                    ent.SendSignal(Signals.prepare_transform);
                    active_entities.Add(ent);
                }
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
            
            // Collision
            List<EntComponent> all_colliders = EntComponent.GetAllOfType(typeof(Collider));
            foreach(Collider collider in all_colliders.Cast<Collider>())
            {
                if(!collider.Host.Enabled || !collider.Active) continue;
                collider.CheckCollisions(all_colliders);
            }

            // Processing
            OnGameTick();
            foreach(Entity ent in active_entities)
            {
                ent.SendSignal(Signals.apply_physics);
                ent.SendSignal(Signals.update);
            }
            OnPostGameTick();
            foreach(Entity ent in active_entities)
            {
                ent.SendSignal(Signals.post_update);
            }
        }
    }
}