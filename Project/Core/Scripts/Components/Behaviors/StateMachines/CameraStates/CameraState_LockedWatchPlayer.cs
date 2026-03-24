using Godot;
using EntComponents;

namespace StateMachines
{
    public class CameraState_LockedWatchPlayer(Behavior owner) : CameraState(owner)
    {
        public override void OnStateTick(double delta)
        {
            // Focus the camera on player without moving
            CameraAutoFocus(DefaultPlayerLookPosition(), delta);
        }
    }
}