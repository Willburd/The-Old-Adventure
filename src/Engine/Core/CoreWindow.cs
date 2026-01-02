using Silk.NET.Input;
using Silk.NET.Windowing;
using Silk.NET.OpenGL;
using System.Numerics;

namespace Engine
{
    public partial class Core 
    {
        public static IWindow WindowContext {get; private set;}
        
        /// <summary>
        /// Called when the window is created.
        /// </summary>
        private static void HandleWindowLoad()
        {
            IInputContext input = WindowContext.CreateInput();
            for (int i = 0; i < input.Keyboards.Count; i++)
            {
                input.Keyboards[i].KeyDown += HandleKeyDown;
                input.Keyboards[i].KeyUp += HandleKeyUp;
            }
            for (int i = 0; i < input.Mice.Count; i++)
            {
                input.Mice[i].Cursor.CursorMode = CursorMode.Raw;
                input.Mice[i].MouseMove += HandleMouseRawUpdate;
                input.Mice[i].Scroll += HandleMouseWheel;
            }

            // Get the openGL context from the window
            OpenGLContext = WindowContext.CreateOpenGL();
            singleton?.ConfigureGL();
            
            // Set buffering mode
            WindowContext.ShouldSwapAutomatically = false;

            // Load adventure specific assets
            singleton?.LoadBaseAssets();
            singleton?.OnLoadAssets();

            // Request the first frame right away!
            RequestUpdate = true;
            RequestRender = true;

            // Finalize setup with gamespecific postinit.
            singleton?.OnInit();
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
            AssetLoader.UnloadAllAssets(true);
            Entity.DestroyAllEntities();
            OnEnd();
        }
    }
}