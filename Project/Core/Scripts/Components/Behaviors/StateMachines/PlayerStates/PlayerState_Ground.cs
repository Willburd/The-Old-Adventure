using Godot;
using EntComponents;

namespace StateMachines
{
    public class PlayerState_Ground(Behavior owner) : PlayerState(owner)
    {
        protected const float ground_turnrate = 12f;
        protected const float ground_acceleration = 9f;
        protected const float ground_friction = 12f;
        protected const float ground_run_maxspeed = 3.8f;

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

            // Get direction of movement based on the camera
            bool want_decelerate = true;
            Vector3 move_dir = CameraRelativeMoveDirection();
            float move_intensity = move_dir.Length();
            if (move_dir.Length() > 0.1f)
            {
                // Rotate us toward our destination and move
                float turn_mult = Mathf.Clamp(move_dir.Length(), 0.1f, 1f);
                Vector3 goal_rotation = Tools.GetFlatDirEulars(move_dir);
                float result_angle = Mathf.RotateToward(PlayerBehavior.NodeParent.GlobalRotation.Y, goal_rotation.Y, ground_turnrate * turn_mult * (float)delta);
                
                // Flip direction if can
                if(Mathf.RadToDeg(Mathf.Abs(Mathf.AngleDifference(PlayerBehavior.NodeParent.GlobalRotation.Y, goal_rotation.Y))) > 150f && turn_mult > 0.4f) 
                {
                    result_angle = goal_rotation.Y; // Snap angle instantly for turn around
                }
                
                // Apply rotation
                PlayerBehavior.NodeParent.GlobalRotation = new Vector3(0f,result_angle,0f);

                // Accelerate toward facing direction of player
                float move_speed = ground_run_maxspeed * move_intensity;
                if( move_intensity < 0.3f) move_speed = 0f; // Only rotate don't move yet
                _movement_velocity = Tools.StepToward(_movement_velocity, (Vector3.Forward * move_speed).Rotated(Vector3.Up, PlayerBehavior.NodeParent.GlobalRotation.Y), ground_acceleration * (float)delta);

                // skid stop
                want_decelerate = move_dir.Dot(_movement_velocity.Normalized()) < 0;
            }

            // Handle slowing down
            if(want_decelerate)
            {
                _movement_velocity = Tools.Decelerate(_movement_velocity, ground_friction * (float)delta);
            }

            // Handle movement
            StandardHandleMovementVelocity(delta);
        }
    }
}