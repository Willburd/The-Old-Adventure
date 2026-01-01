
namespace Engine
{
    public class EditorCamera : Camera
    {
        public EditorCamera(Transform initial_location, bool is_active, Room? room_link) : base(initial_location, is_active, room_link)
        {
            new EntComponents.EditorCameraBehavior(this);
        }
    }
}