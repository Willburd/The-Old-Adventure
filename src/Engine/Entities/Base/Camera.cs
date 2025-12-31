using System.Numerics;
using Silk.NET.Maths;

namespace Engine
{
    public class Camera : Entity
    {
        public float Zoom = 45f;
        public float AspectRatio { get; set; } = 1.33333333333f;


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



        public static Matrix4x4 GetCurrentInterpolatedViewMatrix(double tick_delta)
        {
            if(world_active_cam == null) return Transform.Identity.ViewMatrix;
            return world_active_cam.GetInterpolatedViewMatrix(tick_delta);
        }

        public static Matrix4x4 GetCurrentViewMatrix()
        {
            if(world_active_cam == null) return Transform.Identity.ViewMatrix;
            return world_active_cam.GetViewMatrix();
        }

        public static Matrix4x4 GetCurrentProjectionMatrix()
        {
            if(world_active_cam == null) return Matrix4x4.CreatePerspectiveFieldOfView(Tools.DegreesToRadians(1f), 1.33333333333f, 0.001f, 400.0f);
            return world_active_cam.GetProjectionMatrix();
        }
        
        public new Matrix4x4 GetInterpolatedViewMatrix(double tick_delta)
        {
            Matrix4x4 mat = Matrix4x4.CreateTranslation(-GetInterpolatedPosition(tick_delta)) * Matrix4x4.CreateFromQuaternion(Quaternion.Inverse(GetInterpolatedRotation(tick_delta))); 
            // Depth fix
            mat.M31 = -mat.M31;
            mat.M32 = -mat.M32;
            mat.M33 = -mat.M33;
            mat.M34 = -mat.M34;
            return mat;
        }

        public new Matrix4x4 GetViewMatrix()
        {
            Matrix4x4 mat = Matrix4x4.CreateTranslation(-transform.Position) * Matrix4x4.CreateFromQuaternion(Quaternion.Inverse(transform.Rotation)); 
            // Depth fix
            mat.M31 = -mat.M31;
            mat.M32 = -mat.M32;
            mat.M33 = -mat.M33;
            mat.M34 = -mat.M34;
            return mat;
        }

        public Matrix4x4 GetProjectionMatrix()
        {
            return Matrix4x4.CreatePerspectiveFieldOfView(Tools.DegreesToRadians(Zoom), AspectRatio, 0.001f, 400.0f);
        }
    }
}