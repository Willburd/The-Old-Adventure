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
            destroy,                // () : Called when an entity is destroyed.
            unittest,               // () : Unit testing hook

            // Update
            pre_update,             // () : Custom entity early update
            editor_update,          // () : EditorMode only update
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
            collision,              // (List<Collider.Collision>) : Called from Collider component, returns all collisions during the game tick.
            trigger,                // (List<Collider.Collision>) : Called from Collider component, returns all trigger during the game tick.
            collision_start,        // (Collider.Collision) : Called from Collider component, for EACH collision started
            trigger_start,          // (Collider.Collision) : Called from Collider component, for EACH trigger entered
            collision_end,          // (Collider) : Called from Collider component, for EACH collision exited
            trigger_end,            // (Collider) : Called from Collider component, for EACH trigger exited

            // Raycasting
            raycast,                // (Collider.Raycast, List<ref Collider.RaycastHit>) : Check for raycast line intersection with colliders

            // Input
            input_pressed,            // (Key, ButtonName) : Fired from Input components with the key that was pressed
            input_released,           // (Key, ButtonName) : Fired from Input components with the key that was released

            // Global signals
            global_room_exit_trigger, // () : Called when a room trigger requests to exit a room
            global_fadeout_begin,     // () : Called when a fadeout begins.
            global_room_unloaded,     // (Room unloading) : Called when a room unloads. Used for special cases where we only want to do an action on room unload and not just Destroy()
            global_fadein_end,        // () : Called when a fadein ends.
            global_room_loaded,       // (Room loaded) : Called when a room loads. Handles room start events.
            global_cutscene_begin,    // (Cutscene begin) : Called when a cutscene begins.
            global_cutscene_end,      // (Cutscene begin) : Called when a cutscene ends
            global_input_pressed,       // (Key key) : Fired from the input handler when a key is pressed
            global_input_released,      // (Key key) : Fired from the input handler when a key is released
            global_all_collisions,    // (List<Collider.Collision>) : Called at the end of collision resolution. Sends a list of all collisions that frame for all colliders
            global_all_triggers,      // (List<Collider.Collision>) : Called at the end of collision resolution. Sends a list of all triggers that frame for all trigger volumes
            global_camera_activated,  // (Camera active_camera) : When a camera becomes the active camera
            global_camera_deactivated, // (Camera deactivated_camera) : When a camera is disabled
            global_unittest            // () : Unit testing hook
        }
    }
}