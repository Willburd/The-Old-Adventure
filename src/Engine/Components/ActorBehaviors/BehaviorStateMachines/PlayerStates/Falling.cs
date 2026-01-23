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
            PhysicsBody phys = (PhysicsBody)GetComponent(typeof(PhysicsBody));
            Collider col = (Collider)GetComponent(typeof(Collider));
            float player_radius = ((CylinderCol)col.CollisionShape).radius;

            // process floors
            if (StandardProcessFloors(phys) != null)
            {
                Player.SetPlayerState(new Grounded(Player));
                return;
            }

            // Get direction of movement based on the camera
            Vector3 move_dir = CameraRelativeMoveDirection();
            float move_intensity = move_dir.Length();
            if (move_dir.Length() > 0f)
            {
                // Rotate us toward our destination and move
                Quaternion goal_rotation = Tools.FlatRotation(move_dir);
                Host.Rotation = Tools.FlatRotation(Quaternion.Lerp(Host.Rotation, goal_rotation, air_turnrate)); // Ensure we don't skew our angle
                phys.Velocity = Tools.Accelerate(phys.Velocity, Vector3.Transform(Tools.Forward, Host.Rotation) * move_intensity * air_acceleration, air_maxspeed);
            }

            // Process walls
            StandardProcessWalls(phys, player_radius, ((CylinderCol)col.CollisionShape).height);
            StandardProcessCeilings(phys, ((CylinderCol)col.CollisionShape).height);
        }
    }
}