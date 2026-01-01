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
            RequestUpdate = true;
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
            
            // Rotate
            if (key == Key.Q)
            {
                Camera.WorldCamera?.Rotation *= Tools.Euler(0f,15f,0f);
            }
            
            if (key == Key.E)
            {
                Camera.WorldCamera?.Rotation *= Tools.Euler(0f,-15f,0f);
            }
            
            // Move
            float camera_speed = 0.6f;
            if (key == Key.W)
            {
                Camera.WorldCamera?.Position += Vector3.Transform(Tools.Forward * camera_speed, Camera.WorldCamera.Location.Rotation);
            }
            
            if (key == Key.S)
            {
                Camera.WorldCamera?.Position += Vector3.Transform(Tools.Backward * camera_speed, Camera.WorldCamera.Location.Rotation);
            }
            if (key == Key.A)
            {
                Camera.WorldCamera?.Position += Vector3.Transform(Tools.Left * camera_speed, Camera.WorldCamera.Location.Rotation);
            }
            
            if (key == Key.D)
            {
                Camera.WorldCamera?.Position += Vector3.Transform(Tools.Right * camera_speed, Camera.WorldCamera.Location.Rotation);
            }

            
            if (key == Key.R)
            {
                Camera.WorldCamera?.Position += Tools.Up * camera_speed;
            }
            
            if (key == Key.F)
            {
                Camera.WorldCamera?.Position += Tools.Down * camera_speed;
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