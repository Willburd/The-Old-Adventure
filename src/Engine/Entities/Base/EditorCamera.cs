namespace Engine
{
    public class EditorCamera : Camera
    {
        public EditorCamera(Transform initial_location, string actor_id, bool is_active, Room? room_link) : base(initial_location, actor_id, is_active, room_link)
        {
            new EntComponents.EditorCameraBehavior(this);
            new EntComponents.Input(this);
            new EntComponents.Collider(this); // Collision debugging
        }

        protected override bool CanBeActivated()
        {
            return Core.EditorMode;
        }
    }
}