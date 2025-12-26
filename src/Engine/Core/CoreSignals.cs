namespace Engine
{
    public partial class Core
    {
        public enum Signals
        {
            test,
            // Spawning
            asset_load,
            create,
            destroy,

            // Update
            pre_update,
            update,
            post_update,

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