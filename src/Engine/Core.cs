using EntComponents;
using Silk.NET.Input;
using Silk.NET.Maths;
using Silk.NET.Windowing;
using System.Diagnostics;

namespace Engine
{
    public class Core
    {
        public enum Signals
        {
            test,

            // Rendering
            render_priority,
            pre_render,
            render,
            hud_render,

            // Collision and triggers
            collision,
            trigger
        }


        private static Core? singleton;
        private readonly static IWindow window_context;

        protected const string adventure_title = "The Old Adventure";

        static Core()
        {
            // Prepare window
            WindowOptions options = WindowOptions.Default with
            {
                Size = new Vector2D<int>(800, 600),
                Title = adventure_title
            };

            // Create window
            window_context = Window.Create(options);
            window_context.Load += HandleWindowLoad;
            window_context.Update += HandleWindowUpdate;
            window_context.Render += HandleWindowRender;
            window_context.Closing += HandleWindowClosing;
        }

        public Core()
        {
            Debug.Assert(singleton == null,"Multiple cores created - " + adventure_title);
            singleton = this;
            Console.WriteLine("CORE INIT - " + adventure_title);

            // Start setup with gamespecific preinit.
            OnPreInit();

            // Load adventure specific assets
            OnLoadAssets();

            // Start the window. Everything from here is handled by HandleWindowUpdate() and HandleWindowRender()
            window_context.Run();
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Window Handlers
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
        private static void HandleWindowLoad()
        {
            IInputContext input = window_context.CreateInput();
            for (int i = 0; i < input.Keyboards.Count; i++)
            {
                input.Keyboards[i].KeyDown += KeyDown;
            }

            // Request the first frame right away!
            RequestRender = true;

            // Finalize setup with gamespecific postinit.
            singleton?.OnInit();
        }

        private static void KeyDown(IKeyboard keyboard, Key key, int keyCode)
        {
            // TEMP, we need an input handler here...
            if (key == Key.Escape)
            {
                window_context.Close();
            }
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // tick and render control
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        /// <summary>
        /// Number of game ticks per second for gameplay updates.
        /// </summary>
        protected static double TICKRATE {get; set;} = 40;
        
        /// <summary>
        /// Number of frames per second for rendering.
        /// </summary>
        private static double FPS {get; set;} = 60;
        
        /// <summary>
        /// The threshold needed for the delta_time accumulator to trigger a gametick.
        /// </summary>
        private static double GameTickInterval {get{ return 1.0 / TICKRATE; }}
        private static double game_tick_accumulator = 0;

        /// <summary>
        /// The threshold needed for the delta_time accumulator to trigger a frame render.
        /// </summary>
        private static double FpsTickInterval {get{ return 1.0 /  FPS; }}
        private static double game_fps_accumulator = 0;
        
        /// <summary>
        /// Skips delta_time check for rendering frames, forcing a frame to be renderer as soon as possible. Used when changing scenes for example.
        /// </summary>
        private static bool RequestRender {get; set;}

        /// <summary>
        /// Number of game ticks since launch.
        /// </summary>
        public static long ElapsedGameTicks {get; set;}

        /// <summary>
        /// Number of renderer frames since launch.
        /// </summary>
        public static long ElapsedGameFrames {get; set;}

        /// <summary>
        /// Percent difference from the previous game tick, to the next gametick. Used to do "inbetween" frames during rendering. 
        /// </summary>
        public static double GameTickDelta {get{ return game_tick_accumulator / GameTickInterval; }}

        
        /// <summary>
        /// Handles calling GameTicks at the desired interval
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
        /// Handles rendering the game at the desired interval
        /// </summary>
        private static void HandleWindowRender(double deltaTime)
        {
            game_fps_accumulator += deltaTime;
            if(game_fps_accumulator >= FpsTickInterval || RequestRender)
            {
                ElapsedGameFrames++;
                // We're effectively lerping between the previous draw and the new draw based on how far the gametick has progressed
                singleton?.RenderTick(GameTickDelta); 
                game_fps_accumulator -= FpsTickInterval;
                RequestRender = false;
            }
        }
        
        /// <summary>
        /// Handles the game shutting down.
        /// </summary>
        private static void HandleWindowClosing()
        {
            // Shutdown
            singleton?.MainLoopEnd();
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Gameloop processing
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
            foreach(Collider collider in all_colliders.Cast<Collider>())
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
        }

        /// <summary>
        /// Render tick, fired at the game's framerate. Sends a render signals to all entities depending on their enabled state.
        /// </summary>
        private void RenderTick(double tick_delta)
        {
            // Assemble a list in order of priority.
            SortedList<int,Entity> render_queue = [];
            foreach(Entity check in Entity.EntityList)
            {
                int priority = check.SendSignal(Signals.render_priority, tick_delta);
                if(priority == 0) continue; // Not visible if no component responds.
                render_queue.Add(priority, check);
                check.SendSignal(Signals.pre_render, tick_delta); // perform prerender while we're here.
            }
            OnPreRenderTick();

            // Primary rendering
            foreach((int key, Entity check) in render_queue)
            {
                check.SendSignal(Signals.render, tick_delta);
            }
            OnRenderTick();
            
            // Hud rendering
            foreach((int key, Entity check) in render_queue)
            {
                check.SendSignal(Signals.hud_render, tick_delta);
            }
            OnRenderHudTick();
        }

        /// <summary>
        /// Fired when the mainloop is ended, and the game is shutting down. Used to safely unload assets and run postgame events.
        /// </summary>
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
        /// Virtual function for adventure specific behaviors. Used to load adventure specific assets during engine init.
        /// </summary>
        public virtual void OnLoadAssets()
        {
            
        }
        
        /// <summary>
        /// Virtual function for adventure specific behaviors. Called after the game window has be started, after all engine setup has completed.
        /// </summary>
        public virtual void OnInit()
        {
            
        }

        /// <summary>
        /// Virtual function for adventure specific behaviors. Called at the start of MainLoopEnd(), to allow cleanup of adventure specific engine code.
        /// </summary>
        public virtual void OnEnd()
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
        /// Virtual function for game specific behaviors. Called at the end of a render tick, after everything is rendered, but before the hud.
        /// </summary>
        public virtual void OnRenderTick()
        {
            
        }

        /// <summary>
        /// Virtual function for game specific behaviors. Called at the end of a render tick, after the hud is rendered.
        /// </summary>
        public virtual void OnRenderHudTick()
        {
            
        }
    }
}