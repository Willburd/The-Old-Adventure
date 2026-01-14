using Engine;
using Engine.ColliderShapes;
using System.Numerics;

namespace EntComponents.ActorBehavior.PlayerStates
{
    public class Falling(PlayerActorBehavior owner) : PlayerState(owner)
    {
        public override void Start(BehaviorStateMachine? previous_state)
        {
            PhysicsBody phys = (PhysicsBody)GetComponent(typeof(PhysicsBody));
            phys.HasGravity = true;
            phys.FlatFriction = air_friction;
        }

        public override void Process()
        {
            Input input = (Input)Host.GetComponent(typeof(Input));
            PhysicsBody phys = (PhysicsBody)GetComponent(typeof(PhysicsBody));
            Collider col = (Collider)GetComponent(typeof(Collider));
            float player_radius = ((CylinderCol)col.CollisionShape).radius;

            // Check for floor
            Collider.RaycastHit? hit = FloorCollision();
            if (hit != null)
            {
                // Snap to floor
                Host.Position = new Vector3(hit.Value.HitPosition.X, hit.Value.HitPosition.Y, hit.Value.HitPosition.Z) + new Vector3(0f, -ground_snap_distance, 0f);
                phys.Velocity = new Vector3(phys.Velocity.X, 0f, phys.Velocity.Z);
                Player.SetPlayerState(new Grounded(Player));
                return;
            }

            // Get direction of movement based on the camera
            Vector3 move_dir = Vector3.Transform(input.Move, CameraRotationToPlayer());
            if (move_dir.Length() > 0f)
            {
                // Rotate us toward our destination and move
                Quaternion goal_rotation = Tools.FlatRotation(move_dir);
                Host.Rotation = Tools.FlatRotation(Quaternion.Lerp(Host.Rotation, goal_rotation, ground_turnrate)); // Ensure we don't skew our angle
                phys.Velocity = Tools.Accelerate(phys.Velocity, Vector3.Transform(Tools.Forward, Host.Rotation) * ground_acceleration, air_maxspeed);
            }

            // Process walls
            StandardProcessWalls(phys, player_radius);
        }
    }
}