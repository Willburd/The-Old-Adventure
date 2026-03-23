using Godot;
using EntComponents;

namespace StateMachines
{
    public class PlayerState_Air(Behavior owner) : PlayerState(owner)
    {
        const float TerminalVelocity = 20f;

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
            _gravity_velocity += Game.GetGravity(delta);
            if(_gravity_velocity.Length() > TerminalVelocity) _gravity_velocity = _gravity_velocity.Normalized() * TerminalVelocity;

            // Handle movement
            StandardHandleMovementVelocity(delta);
            GD.Print(PlayerBehavior.PlayerParent.Velocity);
        }
    }
}