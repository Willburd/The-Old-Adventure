using Silk.NET.Input;
using Silk.NET.Maths;
using Silk.NET.Windowing;
using System.Diagnostics;
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
                input.Keyboards[i].KeyDown += KeyDown;
            }

            // Get the openGL context from the window
            OpenGLContext = WindowContext.CreateOpenGL();
            singleton?.ConfigureGL();

            // Load adventure specific assets
            singleton?.LoadBaseAssets();
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
            
            if (key == Key.Left)
            {
                Camera.WorldCamera?.MoveTransform(new Transform(0f, 1f, 0f));
            }
            
            if (key == Key.Right)
            {
                Camera.WorldCamera?.MoveTransform(new Transform(0f, -1f, 0f));
            }
            
            if (key == Key.Up)
            {
                Camera.WorldCamera?.MoveTransform(new Transform( Vector3.Transform(Tools.Forward * 2f, Camera.WorldCamera.Location.Rotation)));
            }
            
            if (key == Key.Down)
            {
                Camera.WorldCamera?.MoveTransform(new Transform( Vector3.Transform(Tools.Backward * 2f, Camera.WorldCamera.Location.Rotation)));
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
            AssetLoader.UnloadAllAssets(true);
            Entity.DestroyAllEntities();
            OnEnd();
        }
    }
}