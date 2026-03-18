
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
            PhysicsBody phys = (PhysicsBody)GetComponent(typeof(PhysicsBody));
            Collider col = (Collider)GetComponent(typeof(Collider));
            float player_radius = ((CylinderCol)col.CollisionShape).radius;

            // process floors
            Collider.RaycastHit? floorhit = StandardProcessFloors(phys);
            if (floorhit == null)
            {
                Player.SetPlayerState(new Falling(Player));
                return;
            }

            // Slide down hills
            float slip = FloorSlipFactor(floorhit.Value.Normal);

            // TODO - Get material properties to decide slip threshold

            if (slip >= slip_threshold)
            {
                Player.SetPlayerState(new Slipping(Player));
                return;
            }

            // Get direction of movement based on the camera
            Vector3 move_dir = CameraRelativeMoveDirection();
            float move_intensity = move_dir.Length();
            if (move_dir.Length() > 0f)
            {
                // Rotate us toward our destination and move
                Quaternion goal_rotation = Tools.FlatRotation(move_dir);
                Host.Rotation = Tools.FlatRotation(Quaternion.Lerp(Host.Rotation, goal_rotation, ground_turnrate)); // Ensure we don't skew our angle
                phys.Velocity = Tools.Accelerate(phys.Velocity, Vector3.Transform(Tools.Forward * move_intensity, Host.Rotation) * ground_acceleration, ground_run_maxspeed);
            }

            // Process walls and ceilings
            StandardProcessWalls(phys, player_radius, ((CylinderCol)col.CollisionShape).height);
            StandardProcessCeilings(phys, ((CylinderCol)col.CollisionShape).height);
        }
    }
}