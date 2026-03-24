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

            // Recalculate camera vector from input
            Vector2 cam_input_dir = CameraInput(delta);
            if(cam_input_dir.Length() > 0.01f)
            {
                _camera_vector = (_camera_vector + (new Vector3(cam_input_dir.X,cam_input_dir.Y,0f) * Tools.GetDirQuaternion(CameraDirVec()))).Normalized();
            }

            // Calculate the desired distance from our vector
            Vector3 calculated_vector = _camera_vector * DefaultCameraDistance;

            Vector3 goal_pos = GetTarget() + calculated_vector;
            CameraMove(goal_pos, delta, DefaultCameraSpeed);
        }
    }
}