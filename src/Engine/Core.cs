using EntComponents;

namespace Engine
{
    public class Core
    {
        public enum Signals
        {
            test,

            // Rendering
            render_standard,
            render_on_disabled,

            // Collision and triggers
            collision,
            trigger
        }

        protected AssetLoader asset_loader;

        public Core()
        {
            // Start setup with gamespecific preinit.
            OnPreInit();

            // Prepare asset loader and load the game's assets.
            asset_loader = new();

            // Load adventure specific assets
            OnLoadAssets();

            // Finalize setup with gamespecific postinit.
            OnInit();

            // Enter gameloop
            MainLoopHandler();
            
            // Shutdown
            MainLoopEnd();
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Gameloop processing
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
        protected const int game_tick_rate = 20;
        protected long tick_count = 0;
        long GameTickInterval {get{ return 1000 / game_tick_rate * 10000; }}
        long FpsTickInterval {get{ return 1000 /  Renderer.FPS * 10000; }}

        /// <summary>
        /// Gameloop handler. Keeps game timing, and calls the various functions that make up the main gameloop.
        /// </summary>
        public void MainLoopHandler()
        {
            long game_accum = 0;
            long fps_accumulator = 0;

            bool quit = false;
            long prevTicks = DateTime.Now.Ticks;
            while (!quit)
            {
                long ticks = DateTime.Now.Ticks;
                long delta = ticks - prevTicks;
                prevTicks = ticks;

                // Update the game at a fixed rate 
                long game_tick_interval = GameTickInterval;
                game_accum += delta;
                if (game_accum >= game_tick_interval)
                {
                    GameTick();
                    tick_count += 1;
                    game_accum -= game_tick_interval;
                }

                // Render at much faster rates, but use the current progress toward the next gametick as the delta_time for animations
                long fps_tick_interval = FpsTickInterval;
                fps_accumulator += delta;
                if (fps_accumulator >= fps_tick_interval)
                {
                    double delta_time = game_accum / game_tick_interval; // Use the current game tick here, not the renderer's
                    RenderTick(delta_time);
                    fps_accumulator -= fps_tick_interval;
                }

                if(tick_count > 500) quit = true;
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
                ent.OnPreProcess(ent.Enabled);
                if(ent.Enabled) active_entities.Add(ent);
            }

            // Collision
            List<EntComponent> all_colliders = EntComponent.GetAllOfType(typeof(Collider));
            foreach(Collider collider in all_colliders)
            {
                if(!collider.Host.Enabled || !collider.Active) continue;
                collider.CheckCollisions(all_colliders);
            }

            // Processing
            OnGameTick();
            foreach(Entity ent in active_entities)
            {
                ent.OnProcess();
            }

            // Render queue prepare
            Renderer.Prepare();
        }

        /// <summary>
        /// Game tick fired. This processes all game objects, and any special logic during them.
        /// </summary>
        private void RenderTick(double delta_time)
        {
            OnPreRenderTick();

            // Draw render queue
            Renderer.Fire(delta_time);

            OnRenderTick();
        }

        public void MainLoopEnd()
        {
            AssetLoader.UnloadAllAssets();
            Entity.DestroyAllEntities();
            OnEnd();
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Virtual functions
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Virtual function for game specific behaviors. Called at the start of the Engine.Core's constructor, before anything else happens.
        /// </summary>
        public virtual void OnPreInit()
        {
            
        }
        
        /// <summary>
        /// Virtual function for adventure specific behaviors. Called at the end of the Engine.Core's constructor, after all engine setup has completed.
        /// </summary>
        public virtual void OnInit()
        {
            
        }

        /// <summary>
        /// Virtual function for adventure specific behaviors. Called at the start of the Engine.Core's destructor, to allow cleanup of adventure specific engine code.
        /// </summary>
        public virtual void OnEnd()
        {
            
        }

        /// <summary>
        /// Virtual function for adventure specific behaviors. Used to load adventure specific assets during engine init.
        /// </summary>
        public virtual void OnLoadAssets()
        {
            
        }

        /// <summary>
        /// Virtual function for game specific behaviors. Called during the start of a game tick, before anything is processed.
        /// </summary>
        public virtual void OnPreGameTick()
        {
            
        }

        /// <summary>
        /// Virtual function for game specific behaviors. Called at the end of a game tick, after everything is processed.
        /// </summary>
        public virtual void OnGameTick()
        {
            
        }
        
        /// <summary>
        /// Virtual function for game specific behaviors. Called during the start of a render tick, before anything is rendered.
        /// </summary>
        public virtual void OnPreRenderTick()
        {
            
        }

        /// <summary>
        /// Virtual function for game specific behaviors. Called at the end of a render tick, after everything is rendered.
        /// </summary>
        public virtual void OnRenderTick()
        {
            
        }
    }
}