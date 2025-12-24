namespace Engine
{
    public static class Renderer
    {
        private static int assigned_fps;

        static Renderer()
        {
            assigned_fps = 60;
        }
        
        public static void Init()
        {
            
        }

        public static void End()
        {
            
        }

        public static int FPS
        {
            get
            {
                return assigned_fps;
            }
        }
    }
}