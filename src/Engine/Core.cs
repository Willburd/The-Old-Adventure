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
        
        protected const double game_tick_rate = 40;
        private static double FPS {get; set;} = 60;
        private static double game_tick_accumulator = 0;
        private static double game_fps_accumulator = 0;
        private static double GameTickInterval {get{ return 1.0 / game_tick_rate; }}
        private static double FpsTickInterval {get{ return 1.0 /  FPS; }}
        private static bool RequestRender {get; set;}
        private static long tick_count = 0;
        private static long frame_count = 0;
        public static long ElapsedGameTicks {get{ return tick_count; }}
        public static long ElapsedGameFrames {get{ return frame_count; }}

        
        /// <summary>
        /// Handles calling GameTicks at the desired interval
        /// </summary>
        private static void HandleWindowUpdate(double deltaTime)
        {
            game_tick_accumulator += deltaTime;
            if(game_tick_accumulator >= GameTickInterval)
            {
                tick_count++;
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
                frame_count++;
                singleton?.RenderTick(deltaTime);
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
        private void RenderTick(double delta_time)
        {
            // Assemble a list in order of priority.
            SortedList<int,Entity> render_queue = [];
            foreach(Entity check in Entity.EntityList)
            {
                int priority = check.SendSignal(Signals.render_priority, delta_time);
                if(priority == 0) continue; // Not visible if no component responds.
                render_queue.Add(priority, check);
                check.SendSignal(Signals.pre_render, delta_time); // perform prerender while we're here.
            }
            OnPreRenderTick();

            // Primary rendering
            foreach((int key, Entity check) in render_queue)
            {
                check.SendSignal(Signals.render, delta_time);
            }
            OnRenderTick();
            
            // Hud rendering
            foreach((int key, Entity check) in render_queue)
            {
                check.SendSignal(Signals.hud_render, delta_time);
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