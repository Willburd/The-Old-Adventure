namespace Engine
{
    public class PlayerCamera : Camera
    {
        public PlayerCamera(Transform initial_location, string actor_id, bool is_active, Room? room_link) : base(initial_location, actor_id, is_active, room_link)
        {
            new EntComponents.ActorBehavior.PlayerCameraBehavior(this);
            new EntComponents.Input(this);
        }
    }
}