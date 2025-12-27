namespace Engine
{
    public partial class Core
    {
        public enum Signals
        {
            test,
            // Spawning
            asset_load,             // () : Called during entity OnInit() before anything else. Used to load assets.
            create,                 // () : Called after asset load during OnInit()
            cache_components,      // () : Called after create during OnInit(). Allows for components to cache each other for quicker use. Done as it's own step to ensure all components have finished being added.
            destroy,                // () : Called when an entity is destroyed.

            // Update
            prepare_transform,      // () : Update last entity position for draw code
            pre_update,             // () : Custom entity early update
            apply_physics,          // () : Apply physics from components to transforms
            update,                 // () : Custom entity update
            post_update,            // () : ustom entity late update

            // Rendering
            render_priority,        // () : Must be registered, for rendering to know the priority of what is being drawn
            pre_render,             // (double tick_delta) : Custom entity early render
            render,                 // (double tick_delta) : Custom entity render
            post_render,            // (double tick_delta) : Custom entity early render
            hud_render,             // (double tick_delta) : Custom hud render

            // Collision and triggers
            collision,              // (List<Collider>) : Called from Collider component, returns all collisions during the game tick.
            trigger                 // (List<Collider>) : Called from Collider component, returns all collisions during the game tick.
        }
    }
}