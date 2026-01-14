namespace EntComponents.ActorBehavior.PlayerStates
{
    public class PlayerState(PlayerActorBehavior owner) : BehaviorStateMachine(owner)
    {
        public const float ground_acceleration = 0.022f;
        public const float ground_friction = 0.014f;
        public const float ground_run_maxspeed = 0.1f;
        public const float ground_snap_threshold = 0.02f;
    }
}