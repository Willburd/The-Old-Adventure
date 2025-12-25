using Silk.NET.Input;
using Silk.NET.Maths;
using Silk.NET.Windowing;
using System.Diagnostics;
using System.Drawing;
using Silk.NET.OpenGL;

namespace Engine
{
    public partial class Core 
    {
        private static Core? singleton;
        protected const string adventure_title = "The Old Adventure";

        public static IWindow WindowContext {get; set;}
        
#pragma warning disable CS8618 // I don't care if you're upset the static constructor doesn't set it. We're doing it on instantilize.
        public static GL OpenGLContext {get; set;}
        static Core()
        {
            // Prepare window
            WindowOptions options = WindowOptions.Default with
            {
                Size = new Vector2D<int>(800, 600),
                Title = adventure_title
            };

            // Create window
            WindowContext = Window.Create(options);
            WindowContext.Load += HandleWindowLoad;
            WindowContext.Update += HandleWindowUpdate;
            WindowContext.Render += HandleWindowRender;
            WindowContext.Closing += HandleWindowClosing;
        }
#pragma warning restore CS8618

        public Core()
        {
            Debug.Assert(singleton == null,"Multiple cores created - " + adventure_title);
            singleton = this;
            Console.WriteLine("CORE INIT - " + adventure_title);

            // Start setup with gamespecific preinit.
            OnPreInit();

            // Start the window. Everything from here is handled by HandleWindowUpdate() and HandleWindowRender()
            WindowContext.Run();
            WindowContext.Dispose();
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Window Handlers
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
        /// <summary>
        /// Called when the window is created.
        /// </summary>
        private static void HandleWindowLoad()
        {
            IInputContext input = WindowContext.CreateInput();
            for (int i = 0; i < input.Keyboards.Count; i++)
            {
                input.Keyboards[i].KeyDown += KeyDown;
            }

            // Get the openGL context from the window
            OpenGLContext = WindowContext.CreateOpenGL();
            OpenGLContext?.ClearColor(Color.CornflowerBlue);

            // Load adventure specific assets
            AssetLoader.Init();
            singleton?.OnLoadAssets();

            // Request the first frame right away!
            RequestRender = true;

            // Finalize setup with gamespecific postinit.
            singleton?.OnInit();
        }

        /// <summary>
        /// Handles keyboard inputs from the window.
        /// </summary>
        private static void KeyDown(IKeyboard keyboard, Key key, int keyCode)
        {
            // TEMP, we need an input handler here...
            if (key == Key.Escape)
            {
                WindowContext.Close();
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
        // Audio Handlers
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
}