using Godot;
using EntComponents;

namespace StateMachines
{
    public class CameraState_FollowPlayer(Behavior owner) : CameraState(owner)
    {
        public override void OnStateTick(double delta)
        {
            Vector3 player_pos = Game.CurrentPlayer.GlobalPosition + (Vector3.Up * 0.5f);

            // Move the camera into place
            Vector3 goal_pos = player_pos + (Vector3.Back * 5f) + (Vector3.Up * 1f);
            CameraMove(goal_pos, delta, 15f);

            // Focus the camera on target
            TargetMove(player_pos, delta, 25f);
            CameraLookAt(GetTarget(), Vector3.Up);
        }
    }
}