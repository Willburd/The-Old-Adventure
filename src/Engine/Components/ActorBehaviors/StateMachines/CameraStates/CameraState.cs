using Engine;

namespace EntComponents.ActorBehavior.CameraStates
{
    public class CameraState(PlayerCameraBehavior owner) : BehaviorStateMachine(owner)
    {
        protected float distance_from_player = 4.4f;
    }
}