using Godot;
using EntComponents;

namespace StateMachines
{
    public class PlayerState_Ground(Behavior owner) : PlayerState(owner)
    {
        public override void Begin(StateMachine previous_state)
        {
            if (previous_state is PlayerState ply_prev)
            {
                _movement_velocity = ply_prev._movement_velocity;
                _knockback_velocity = ply_prev._knockback_velocity;
                _sliding_velocity = Vector3.Zero;
                _gravity_velocity = Game.GetGravity(1f);
            }
        }

        public override void OnStateTick(double delta)
        {
            if(!PlayerBehavior.OnGround())
            {
                // Dropped off the ground
                PlayerBehavior.CurrentState = new PlayerState_Air(PlayerBehavior);
                return;
            }
            // Handle gravity (we just want a small push)
            _gravity_velocity = Game.GetGravity(delta);

            _movement_velocity = Vector3.Left * 5f;

            // Handle movement
            StandardHandleMovementVelocity(delta);
        }
    }
}