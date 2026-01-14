
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
            phys.FlatFriction = ground_friction;
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
                // Get direction of movement based on the camera
                Vector3 move_dir = Vector3.Transform(input.Move, CameraRotationToPlayer());
                if (move_dir.Length() > 0f)
                {
                    // Rotate us toward our destination and move
                    Quaternion goal_rotation = Tools.FlatRotation(move_dir);
                    if (on_ground) Host.Rotation = Tools.FlatRotation(Quaternion.Lerp(Host.Rotation, goal_rotation, ground_turnrate)); // Ensure we don't skew our angle
                    phys.Velocity = Tools.Accelerate(phys.Velocity, Vector3.Transform(Tools.Forward, Host.Rotation) * ground_acceleration, ground_run_maxspeed);
                }
            }




            // Wallblocking
            
        }
    }
}