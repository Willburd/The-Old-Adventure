
using Engine;
using Engine.ColliderShapes;
using System.Numerics;

namespace EntComponents.ActorBehavior.PlayerStates
{
    public class Slipping(PlayerActorBehavior owner) : PlayerState(owner)
    {
        public override void Start(BehaviorStateMachine? previous_state)
        {
            PhysicsBody phys = (PhysicsBody)GetComponent(typeof(PhysicsBody));
            phys.HasGravity = true;
            phys.FlatFriction = ground_friction * 0.5f;
        }


        public override void Process()
        {
            Input input = (Input)Host.GetComponent(typeof(Input));
            PhysicsBody phys = (PhysicsBody)GetComponent(typeof(PhysicsBody));
            Collider col = (Collider)GetComponent(typeof(Collider));
            float player_radius = ((CylinderCol)col.CollisionShape).radius;

            // Check for floor
            Collider.RaycastHit? hit = FloorCollision();
            if (hit == null)
            {
                // TODO - Enter falling state

                return;
            }

            // Snap to floor
            Host.Position = new Vector3(hit.Value.HitPosition.X, hit.Value.HitPosition.Y, hit.Value.HitPosition.Z) + new Vector3(0f, -ground_snap_distance, 0f);
            phys.Velocity = new Vector3(phys.Velocity.X, 0f, phys.Velocity.Z);

            // Process walls
            StandardProcessWalls(phys, player_radius);

            // Slide down hills
            float slip = FloorSlipFactor(hit.Value.Normal);

            // TODO - Get material properties to decide slip threshold

            // End slip
            if (slip < 0.03f)
            {
                Player.SetPlayerState(new Grounded(Player));
                return;
            }

            // Keep slipping
            phys.Velocity = Tools.Accelerate(phys.Velocity, Tools.FlatDirVector(Vector3.Zero, hit.Value.Normal) * slip_acceleration, slip_maxspeed);
        }
    }
}