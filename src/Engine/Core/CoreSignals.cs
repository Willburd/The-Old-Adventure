namespace Engine
{
    public partial class Core
    {
        public enum Signals
        {
            test,
            // Spawning
            load_assets,            // () : Called just before the create signal, ensuring all assets are loaded
            create,                 // () : Called after asset load during OnInit()
            cache_components,       // () : Called after create during OnInit(). Allows for components to cache each other for quicker use. Done as it's own step to ensure all components have finished being added.
            destroy,                // () : Called when an entity is destroyed.

            // Update
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

            // Movement
            move_absolute,          // (Transform new_location) : Sets the transform component's position
            move_relative,          // (Transform changing_by) : Adds the provided transform to the current transform components's position

            // Collision and triggers
            collision,              // (List<Collider>) : Called from Collider component, returns all collisions during the game tick.
            trigger,                // (List<Collider>) : Called from Collider component, returns all collisions during the game tick.

            // Global signals
            global_fadeout_begin,     // () : Called when a fadeout begins.
            global_room_unloaded,     // (Room unloading) : Called when a room unloads. Used for special cases where we only want to do an action on room unload and not just Destroy()
            global_fadein_end,        // () : Called when a fadein ends.
            global_room_loaded,       // (Room loaded) : Called when a room loads. Handles room start events.
            global_cutscene_begin,    // (Cutscene begin) : Called when a cutscene begins.
            global_cutscene_end,      // (Cutscene begin) : Called when a cutscene ends
        }
    }
}