using System.Numerics;

namespace Engine
{
    public class Camera : Actor
    {
        public static float default_fov = 45f;
        public static float default_aspect = 1.66666667f;
        public static float near_clip = 0.01f;
        public static float far_clip = 1000f;

        public float FieldOfView { get; set; } = default_fov;
        public float AspectRatio { get; set; } = default_aspect;


        private static readonly List<Camera> all_cameras = [];
        public Camera(Transform initial_location, bool is_active, Room? room_link) : base(initial_location, "Engine::Camera", room_link)
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

        protected virtual bool CanBeActivated()
        {
            return !Core.EditorMode;
        }

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
                    // If something blocks us from taking vision
                    if(!CanBeActivated()) return;

                    // Disable previous camera if
                    if(world_active_cam != null && world_active_cam != this)
                    {
                        world_active_cam.camera_active = false;
                        SendGlobalSignal(Core.Signals.global_camera_deactivated, world_active_cam);
                    }

                    // Enable just us
                    camera_active = value;
                    world_active_cam = this;
                    SendGlobalSignal(Core.Signals.global_camera_activated, this);
                    return;
                }
                
                // Disable
                if(camera_active) 
                {
                    camera_active = false;
                    SendGlobalSignal(Core.Signals.global_camera_deactivated, this);
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
            if(world_active_cam == null) return Matrix4x4.CreatePerspectiveFieldOfView(Tools.DegreesToRadians(1f), default_aspect, near_clip, far_clip);
            return world_active_cam.GetProjectionMatrix();
        }
        
        public new Matrix4x4 GetInterpolatedViewMatrix(double tick_delta)
        {
            Matrix4x4.Invert(Matrix4x4.CreateTranslation(GetInterpolatedPosition(tick_delta)), out Matrix4x4 pos);
            Matrix4x4 mat = pos * Matrix4x4.CreateFromQuaternion(Quaternion.Inverse(GetInterpolatedRotation(tick_delta))); 
            return mat;
        }

        public new Matrix4x4 GetViewMatrix()
        {
            Matrix4x4.Invert(Matrix4x4.CreateTranslation(transform.Position), out Matrix4x4 pos);
            Matrix4x4 mat = pos * Matrix4x4.CreateFromQuaternion(Quaternion.Inverse(transform.Rotation)); 
            return mat;
        }

        public Matrix4x4 GetProjectionMatrix()
        {
            return Matrix4x4.CreatePerspectiveFieldOfView(Tools.DegreesToRadians(FieldOfView), AspectRatio, near_clip, far_clip);
        }
    }
}