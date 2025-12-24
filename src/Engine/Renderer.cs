using EntComponents;

namespace Engine
{
    public static class Renderer
    {
        private static List<Renders> render_queue;

        private static int assigned_fps;

        static Renderer()
        {
            render_queue = [];
            assigned_fps = 60;
        }

        /// <summary>
        /// Prepares the current render queue. Checks all Renders ActorComponents, and scenes for if they are valid to render, and adds them to the queue if they are.
        /// </summary>
        public static void Prepare()
        {
            ClearQueue();
            foreach(Renders check in EntComponent.GetAllOfType(typeof(Renders)).Cast<Renders>())
            {
                if(check.Visible) render_queue.Add(check);
            }
        }

        /// <summary>
        /// Draws the current render queue.
        /// </summary>
        public static void Fire(double delta_time)
        {
            foreach(Renders check in render_queue)
            {
                check.Host.SendSignal(check.Host.Enabled ? Core.Signals.render_standard : Core.Signals.render_on_disabled, delta_time);
            }
        }

        /// <summary>
        /// Clears the renderer queue with an empty list.
        /// </summary>
        public static void ClearQueue()
        {
            render_queue = [];
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