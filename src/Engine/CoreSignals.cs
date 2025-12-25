namespace Engine
{
    public partial class Core
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
    }
}