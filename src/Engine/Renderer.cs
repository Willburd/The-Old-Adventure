using EntComponents;

namespace Engine
{
    public class Renderer()
    {
        private List<Renders> render_queue = [];

        private int assigned_fps = 60;

        /// <summary>
        /// Prepares the current render queue. Checks all Renders ActorComponents, and scenes for if they are valid to render, and adds them to the queue if they are.
        /// </summary>
        public void Prepare()
        {
            ClearQueue();
            foreach(Renders check in EntComponent.GetAllOfType(typeof(Renders)).Cast<Renders>())
            {
                render_queue.Add(check);
            }
        }

        /// <summary>
        /// Draws the current render queue.
        /// </summary>
        public void Fire(double delta_time)
        {

            
        }

        /// <summary>
        /// Clears the renderer queue with an empty list.
        /// </summary>
        public void ClearQueue()
        {
            render_queue = [];
        }

        public int FPS
        {
            get
            {
                return assigned_fps;
            }
        }
    }
}