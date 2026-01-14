using System.Numerics;
using Engine;

namespace EntComponents.ActorBehavior.PlayerStates
{
    public class PlayerState(PlayerActorBehavior owner) : BehaviorStateMachine(owner)
    {
        protected const float ground_turnrate = 0.12f;
        protected const float ground_acceleration = 0.022f;
        protected const float ground_friction = 0.014f;
        protected const float ground_run_maxspeed = 0.1f;
        protected const float ground_snap_threshold = 0.02f;

        protected Quaternion CameraRotationToPlayer()
        {
            Vector3 campos = Camera.WorldCamera?.Position ?? Vector3.Zero;
            return Tools.FlatRotation(Tools.DirVector(campos, Host.Position));
        }
    }
}