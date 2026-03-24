using Godot;
using EntComponents;

namespace StateMachines
{
    public class PlayerState(Behavior owner) : StateMachine(owner)
    {
        // Slipping
        protected const float slip_threshold = 0.3f;
        protected const float slip_acceleration = 0.56f;
        protected const float slip_maxspeed = 1.5f;

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

        protected void StandardHandleMovementVelocity(double delta)
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

        protected Vector3 CameraRelativeMoveDirection()
        {
            Vector2 input = Input.GetVector("game_left", "game_right", "game_up", "game_down", 0.05f);
            // Cutscenes ignore camera move
            // if (Cutscenes.Cutscene.Current != null) return input.Move;
            // Camera based movement input
            return new Vector3(input.X, 0f, input.Y).Rotated(Vector3.Up, CameraRotationToPlayer().Y);
        }

        protected Vector3 CameraRotationToPlayer()
        {
            return new Transform3D().LookingAt(Tools.DirVector(Game.WorldCamera.GlobalPosition, PlayerBehavior.NodeParent.GlobalPosition)).Basis.GetEuler();
        }
    }
}