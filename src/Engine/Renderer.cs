using Silk.NET.Input;
using Silk.NET.Maths;
using Silk.NET.Windowing;

namespace Engine
{
    public static class Renderer
    {
        private static int assigned_fps;
        private readonly static IWindow window_context;

        static Renderer()
        {
            assigned_fps = 60;

            // Prepare window
            WindowOptions options = WindowOptions.Default with
            {
                Size = new Vector2D<int>(800, 600),
                Title = "The Old Adventure"
            };

            // Create window
            window_context = Window.Create(options);
            window_context.Run();
            window_context.Load += HandleWindowLoad;
            window_context.Update += HandleWindowUpdate;
            window_context.Render += HandleWindowRender;
        }
        
        public static void Init()
        {
            Console.WriteLine("RENDERER INIT");
            
        }

        public static void End()
        {
            window_context.Close();
        }

        public static int FPS
        {
            get
            {
                return assigned_fps;
            }
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
        }

        private static void HandleWindowUpdate(double deltaTime)
        {
            
        }

        private static void HandleWindowRender(double deltaTime)
        {
            
        }

        private static void KeyDown(IKeyboard keyboard, Key key, int keyCode)
        {
            if (key == Key.Escape)
            {
                window_context.Close();
            }
        }
    }
}