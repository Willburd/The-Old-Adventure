using Godot;
using EntComponents;

namespace StateMachines
{
    public class CameraState(Behavior owner) : StateMachine(owner)
    {
        public const float DefaultCameraDistance = 7f;
        public const float DefaultCameraSpeed = 25f;
        public const float DefaultFocusSpeed = 30f;
        public const float DefaultCameraRotationSpeed = 1.2f;

        public CameraBehavior CameraBehavior
        { 
            get
            {
                return Behavior as CameraBehavior;
            }
        }

        public void TargetMove(Vector3 goal, double delta, float rate = float.PositiveInfinity)
        {
            Behavior.NodeParent.GlobalPosition = Tools.StepToward(Behavior.NodeParent.GlobalPosition, goal, rate * (float)delta);
        }

        public Vector3 GetTarget()
        {
            return Behavior.NodeParent.GlobalPosition;
        }
        
        public Vector3 GetCameraPosition()
        {
            return Game.WorldCamera.GlobalPosition;
        }

        public void CameraMove(Vector3 goal, double delta, float rate = float.PositiveInfinity)
        {
            Game.WorldCamera.GlobalPosition = Tools.StepToward(Game.WorldCamera.GlobalPosition, goal, rate * (float)delta);
        }

        public void CameraAutoFocus(Vector3 goal, double delta, float max_speed = DefaultFocusSpeed)
        {
            TargetMove(goal, delta, max_speed);
            CameraLookAt(GetTarget(), Vector3.Up);
        }

        public void CameraLookAt(Vector3 target, Vector3 up)
        {
            if(target == GetCameraPosition()) return;
            Game.WorldCamera.LookAt(target, up);
        }

        public Vector3 CameraDirVec()
        {
            return Vector3.Forward * Game.WorldCamera.Quaternion.Normalized();
        }

        public Vector3 DefaultPlayerLookPosition()
        {
            return Game.CurrentPlayer.GlobalPosition + (Vector3.Up * 0.35f);
        }
        
        protected Vector2 CameraInput(double delta)
        {
            Vector2 input = Input.GetVector("cam_left", "cam_right", "cam_up", "cam_down", 0.05f) * DefaultCameraRotationSpeed * (float)delta; // Analogs need delta
            // Mouse does not use delta time!
            if(_last_mouse_input.Length() > 0f)
            {
                input = _last_mouse_input;
            }
            _last_mouse_input = Vector2.Zero; // Reset mouse
            // Check inversion settings
            bool invertedX = true;
            bool invertedY = false;
            if(invertedX) input.X = -input.X;
            if(invertedY) input.Y = -input.Y;
            return input;
        }

        protected Vector3 RaycastWallPush(Vector3 calculated_vector)
        {
            RayCast3D ray = CameraBehavior.CameraRay;
            ray.GlobalPosition = GetTarget();
            ray.TargetPosition = calculated_vector;
            if(ray.IsColliding())
            {
                Vector3 final_vec = Tools.VectorTo(ray.GlobalPosition, ray.GetCollisionPoint());
                calculated_vector = final_vec - (final_vec.Inverse().Normalized() * 0.1f); // Buffer away from walls
            }
            return calculated_vector;
        }
        
        
        protected Vector3 ClampMinDistance(Vector3 calculated_vector)
        {
            Vector3 _min_point = Tools.FlatDirVector(calculated_vector) * 0.5f;
            if(Tools.FlatVectorTo(Vector3.Zero, calculated_vector).Length() < _min_point.Length())
            {
                calculated_vector = new Vector3(_min_point.X, calculated_vector.Y, _min_point.Z);
                GD.Print(calculated_vector);
            }
            return calculated_vector;
        }

        protected Vector2 _last_mouse_input = Vector2.Zero;
        public override void InputHandler(InputEvent @event)
        {
			if (@event is InputEventMouseMotion eventMouseMotion)
			{
				_last_mouse_input = (eventMouseMotion.Relative / 100f) * DefaultCameraRotationSpeed;
			}
        }
    }
}