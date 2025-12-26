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
        public static long ElapsedGameTicks {get; set;}

        /// <summary>
        /// Percent difference from the previous game tick, to the next gametick. Used to do "inbetween" frames during rendering. 
        /// </summary>
        public static double GameTickDelta {get{ return game_tick_accumulator / GameTickInterval; }}


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
            // Preprocessing
            OnPreGameTick();
            List<Entity> active_entities = [];
            foreach(Entity ent in Entity.EntityList)
            {
                if(!ent.IsInitilized) ent.OnInit(); // Actually setup entites, needed for create and asset loading signals.
                uint has_update_registered = ent.SendSignal(Signals.pre_update, ent.Enabled);
                if(ent.Enabled && has_update_registered > 0) active_entities.Add(ent);
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