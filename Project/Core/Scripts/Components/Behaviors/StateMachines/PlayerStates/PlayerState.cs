using Godot;
using EntComponents;

namespace StateMachines
{
    public class PlayerState(Behavior owner) : StateMachine(owner)
    {
        public Vector3 _movement_velocity = Vector3.Zero;
        public Vector3 _knockback_velocity = Vector3.Zero;
        public Vector3 _sliding_velocity = Vector3.Zero;
        public Vector3 _gravity_velocity = Vector3.Zero;

        public PlayerBehavior PlayerBehavior
        { 
            get
            {
                return Behavior as PlayerBehavior;
            }
        }

        public override void OnStatePhysicsTick(double delta)
        {
            PlayerBehavior.PlayerParent.MoveAndSlide();
        }

        public Vector3 GetGravity(double delta)
        {
            return (Vector3)ProjectSettings.GetSetting("physics/3d/default_gravity") * (float)delta;
        }

        public void StandardHandleMovementVelocity(double delta)
        {
            // We are on the ground and ready to move!
            Vector3 final_vel = _movement_velocity;
            final_vel += _knockback_velocity;
            // final_vel += _sliding_velocity; // Needs to do a dotproduct to stop it from applying while you are running in the same direction
            final_vel += _gravity_velocity;
            PlayerBehavior.PlayerParent.Velocity = final_vel;

            // Reduce knockback
            _knockback_velocity = Tools.Decelerate(_knockback_velocity, 1f * (float)delta);
            _sliding_velocity = Tools.Decelerate(_sliding_velocity, 0.2f * (float)delta);
        }
    }
}