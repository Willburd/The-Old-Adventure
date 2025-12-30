using System.Numerics;

namespace Engine
{
    public class Camera : Entity
    {
        public float Zoom = 1f;
        public float AspectRatio { get; set; } = 1.66666f;


        private static readonly List<Camera> all_cameras = [];
        public Camera(Transform initial_location, bool is_active) : base(initial_location)
        {
            all_cameras.Add(this);
            IsActive = is_active;
        }
        protected override void OnCleanup()
        {
            all_cameras.Remove(this);
        }


        private static Camera? world_active_cam = null;

        public static Camera? WorldCamera
        {
            get
            {
                return world_active_cam;
            }
        }


        private bool camera_active = false;

        public bool IsActive
        {
            get
            {
                return camera_active;
            }   

            set
            {
                // Enable
                if(value)
                {
                    // Disable previous camera if
                    if(world_active_cam != null && world_active_cam != this)
                    {
                        world_active_cam.camera_active = false;
                        SendGlobalSignal(Core.Signals.camera_deactivated, world_active_cam);
                    }

                    // Enable just us
                    camera_active = value;
                    world_active_cam = this;
                    SendGlobalSignal(Core.Signals.camera_activated, this);
                    return;
                }
                
                // Disable
                if(camera_active) 
                {
                    camera_active = false;
                    SendGlobalSignal(Core.Signals.camera_deactivated, this);
                    world_active_cam = null;
                }
            } 
        }



        public static Matrix4x4 GetCurrentViewMatrix()
        {
            if(world_active_cam == null) return Transform.Identity.ViewMatrix;
            return world_active_cam.GetViewMatrix();
        }

        public static Matrix4x4 GetCurrentProjectionMatrix()
        {
            if(world_active_cam == null) return Matrix4x4.CreatePerspectiveFieldOfView(Tools.DegreesToRadians(1f), 1.66666f, 0.1f, 180.0f);
            return world_active_cam.GetProjectionMatrix();
        }
        
        public new Matrix4x4 GetViewMatrix()
        {
            return Matrix4x4.CreateLookAt(Position, Position + Vector3.Normalize(Vector3.Transform(Tools.Forward, Rotation)), Tools.Up);
        }

        public Matrix4x4 GetProjectionMatrix()
        {
            return Matrix4x4.CreatePerspectiveFieldOfView(Tools.DegreesToRadians(Zoom), AspectRatio, 0.1f, 180.0f);
        }
    }
}