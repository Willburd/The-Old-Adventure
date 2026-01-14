
using Engine;
using System.Numerics;

namespace EntComponents.ActorBehavior.PlayerStates
{
    public class Grounded(PlayerActorBehavior owner) : PlayerState(owner)
    {
        public override void Start(BehaviorStateMachine? previous_state)
        {
            PhysicsBody phys = (PhysicsBody)GetComponent(typeof(PhysicsBody));
            phys.HasGravity = true;
            phys.Friction = new Vector3(ground_friction, 0f, ground_friction);
        }

        public override void Process()
        {
            Input input = (Input)Host.GetComponent(typeof(Input));
            PhysicsBody phys = (PhysicsBody)GetComponent(typeof(PhysicsBody));

            // Check for floor
            bool on_ground = false;
            Collider.RaycastHit? hit = Collider.DoRaycastNearest(Host.Position + Tools.Up, Tools.Down * (1f + ground_snap_threshold), Collider.mask_worldgeo);
            if (hit != null && phys != null)
            {
                // Snap to floor
                Host.Position = new Vector3(hit.Value.HitPosition.X, hit.Value.HitPosition.Y, hit.Value.HitPosition.Z) + new Vector3(0f, -ground_snap_threshold, 0f);
                phys.Velocity = new Vector3(phys.Velocity.X, 0f, phys.Velocity.Z);
                phys.HasGravity = false;
                on_ground = true;
            }
            else
            {
                // Gravity
                phys.HasGravity = true;
                on_ground = false;
            }

            // Movement
            if (input != null)
            {
                // Rotation
                Vector3 move_dir = input.Move;
                Host.Rotation *= Tools.CreateFromAxisAngle(Tools.Up, move_dir.X * -0.1f);
                // Movement
                move_dir.X = 0f;
                phys.Velocity = Tools.Accelerate(phys.Velocity, Vector3.Transform(move_dir, Host.Rotation) * ground_acceleration, ground_run_maxspeed);
            }
        }
    }
}