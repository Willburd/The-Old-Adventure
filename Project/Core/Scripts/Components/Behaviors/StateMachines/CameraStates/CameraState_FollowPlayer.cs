using Godot;
using EntComponents;
namespace StateMachines
{
    public class CameraState_FollowPlayer(Behavior owner) : CameraState(owner)
    {
        private Vector3 _camera_vector = Vector3.Back;

        public override void Begin(StateMachine previous_state)
        {
            TargetMove(DefaultPlayerLookPosition(), 1f);
            _camera_vector = Tools.DirVector(GetTarget(), GetCameraPosition());
        }

        public override void OnStateTick(double delta)
        {
            // Move the camera into place behind the player
            TargetMove(DefaultPlayerLookPosition(), delta);
            CameraLookAt(GetTarget(), Vector3.Up);
            float minimum_bound = 0.2f;

            // Recalculate camera vector from input
            var org_camera_vector = _camera_vector;
            Vector2 cam_input_dir = CameraInput(delta);
            if(cam_input_dir.Length() > 0.001f)
            {
                // Move camera
                _camera_vector = (org_camera_vector + InputToCameraSpace(cam_input_dir)).Normalized();
                // Prevent getting too close by disabling the Y axis if we do!
                if(Tools.FlatVectorTo(Vector3.Zero, _camera_vector).Length() < minimum_bound)
                {
                    _camera_vector = (org_camera_vector + InputToCameraSpace(new Vector2(cam_input_dir.X,0f))).Normalized();
                }
            }

            // Calculate the desired distance from our vector
            Vector3 calculated_vector = _camera_vector * DefaultCameraDistance;
            // calculated_vector = RaycastWallPush(calculated_vector);

            // Apply final end position
            Vector3 goal_pos = GetTarget() + calculated_vector;
            
            /*
            // Forbid positions too close to the player here too
            Vector3 flat_away = Tools.FlatVectorTo(GetTarget(), goal_pos);
            float final_bounds = 0.3f;
            if(flat_away.Length() < final_bounds)
            {
                flat_away = flat_away.Normalized() * final_bounds;
                goal_pos = GetTarget() + new Vector3(flat_away.X, goal_pos.Y, flat_away.Z);
            }
            */

            // Prevent camera goal from being too close to the player above or below
            CameraMove(goal_pos, delta, DefaultCameraSpeed);
            CameraLookAt(GetTarget(), Vector3.Up);
        }
    }
}