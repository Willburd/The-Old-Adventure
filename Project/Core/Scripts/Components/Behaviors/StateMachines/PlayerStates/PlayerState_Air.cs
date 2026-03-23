using Godot;
using EntComponents;

namespace StateMachines
{
    public class PlayerState_Air(Behavior owner) : PlayerState(owner)
    {
        public override void Begin(StateMachine previous_state)
        {
            if (previous_state is PlayerState ply_prev)
            {
                _movement_velocity = ply_prev._movement_velocity;
                _knockback_velocity = ply_prev._knockback_velocity;
                _sliding_velocity = Vector3.Zero;
                _gravity_velocity = ply_prev._gravity_velocity;
            }
        }

        public override void OnStateTick(double delta)
        {
            if(PlayerBehavior.OnGround())
            {
                // Landed on the ground
                PlayerBehavior.CurrentState = new PlayerState_Ground(PlayerBehavior);
                return;
            }
            // slowdown movement in the air
            _movement_velocity = Tools.DecelerateFlat(_movement_velocity, 0.5f * (float)delta);

            // Handle gravity
            _gravity_velocity += GetGravity(delta);

            // Handle movement
            StandardHandleMovementVelocity(delta);
        }
    }
}