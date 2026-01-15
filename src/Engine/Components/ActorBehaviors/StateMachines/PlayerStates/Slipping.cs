
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

            // process collisions
            Collider.RaycastHit? floorhit = StandardProcessFloors(phys);
            if (floorhit == null)
            {
                Player.SetPlayerState(new Falling(Player));
                return;
            }

            // Slide down hills
            float slip = FloorSlipFactor(floorhit.Value.Normal);

            // TODO - Get material properties to decide slip threshold

            // End slip
            if (slip < slip_threshold)
            {
                Player.SetPlayerState(new Grounded(Player));
                return;
            }

            // Keep slipping
            phys.Velocity = Tools.Accelerate(phys.Velocity, Tools.FlatDirVector(Vector3.Zero, floorhit.Value.Normal) * slip_acceleration, slip_maxspeed);

            StandardProcessWalls(phys, player_radius, ((CylinderCol)col.CollisionShape).height);
            StandardProcessCeilings(phys, ((CylinderCol)col.CollisionShape).height);
        }
    }
}