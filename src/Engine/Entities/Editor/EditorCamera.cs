
namespace Engine
{
    public class EditorCamera : Camera
    {
        public EditorCamera(Transform initial_location, bool is_active) : base(initial_location, is_active)
        {
            new EntComponents.EditorCameraBehavior(this);
        }
    }
}