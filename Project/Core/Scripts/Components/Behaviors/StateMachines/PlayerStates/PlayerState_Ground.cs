using Godot;
using EntComponents;

namespace StateMachines
{
    public class PlayerState_Ground(Behavior owner) : PlayerState(owner)
    {
        const float GroundSpeed = 6f;

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

            // Get player input
            Vector2 direction_input = Input.GetVector("game_left", "game_right", "game_up", "game_down", 0.01f);
            _movement_velocity = new Vector3(direction_input.X, 0f, direction_input.Y) * GroundSpeed;

            // Handle movement
            StandardHandleMovementVelocity(delta);
        }
    }
}