
using Engine;
using Engine.ColliderShapes;
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
            Collider col = (Collider)GetComponent(typeof(Collider));
            float player_radius = ((CylinderCol)col.CollisionShape).radius;

            // Check for floor
            Collider.RaycastHit? hit = Collider.DoRaycastNearest(Host.Position + Tools.Up, Tools.Down * (1f + ground_snap_distance), Collider.mask_worldgeo);
            if (hit != null)
            {
                // Snap to floor
                Host.Position = new Vector3(hit.Value.HitPosition.X, hit.Value.HitPosition.Y, hit.Value.HitPosition.Z) + new Vector3(0f, -ground_snap_distance, 0f);
                phys.Velocity = new Vector3(phys.Velocity.X, 0f, phys.Velocity.Z);
                phys.HasGravity = false;

                // Slide down hills
                float slip = FloorSlipFactor(hit.Value.Normal);
                // TODO - Get material properties to decide slip threshold
                if (slip > 0.03f)
                {
                    phys.Velocity = Tools.Accelerate(phys.Velocity, Tools.FlatDirVector(Vector3.Zero, hit.Value.Normal) * slip_acceleration, slip_maxspeed);
                }
            }
            else
            {
                // Dropped off floor, falling time
                phys.HasGravity = true;

                return;
            }

            // Get direction of movement based on the camera
            Vector3 move_dir = Vector3.Transform(input.Move, CameraRotationToPlayer());
            if (move_dir.Length() > 0f)
            {
                // Rotate us toward our destination and move
                Quaternion goal_rotation = Tools.FlatRotation(move_dir);
                Host.Rotation = Tools.FlatRotation(Quaternion.Lerp(Host.Rotation, goal_rotation, ground_turnrate)); // Ensure we don't skew our angle
                phys.Velocity = Tools.Accelerate(phys.Velocity, Vector3.Transform(Tools.Forward, Host.Rotation) * ground_acceleration, ground_run_maxspeed);
            }
            
            // Check upper Wallblocking
            Collider.RaycastHit? upper_hit = WallCollision(phys.Velocity, character_height, player_radius);
            if (upper_hit != null)
            {

                Console.WriteLine("Upper " + upper_hit.Value.Distance);
            }
            else
            {
                // Check lower wall blocking, no pushing here
                Collider.RaycastHit? lower_hit = WallCollision(phys.Velocity, wallcast_y_lower, player_radius);
                if (lower_hit != null)
                {

                    Console.WriteLine("Lower " + lower_hit.Value.Distance);
                }
            }
        }
    }
}