
using Engine;
using System.Numerics;

namespace EntComponents.ActorBehavior.PlayerStates
{
    public class Grounded(PlayerActorBehavior owner) : PlayerState(owner)
    {
        public override void Start(PlayerState? previous_state)
        {
            PhysicsBody? phys = (PhysicsBody?)GetComponent(typeof(PhysicsBody));
            phys?.HasGravity = true;
        }

        public override void Process()
        {
            PhysicsBody? phys = (PhysicsBody?)GetComponent(typeof(PhysicsBody));
            Collider.RaycastHit? hit = Collider.DoRaycastNearest(Host.Position + Tools.Up, Tools.Down * 1.01f, Collider.mask_worldgeo);
            if(hit != null)
            {
                // Snap to floor
                Host.Position = new Vector3(Host.Position.X, (hit.Value.HitPosition + (Tools.Up * 0.01f)).Y, Host.Position.Z);
                phys?.HasGravity = false;
                phys?.Velocity = new Vector3(phys.Velocity.X, 0f, phys.Velocity.Y);
            }
            else
            {
                // Gravity
                phys?.HasGravity = true;
            }
        }
    }
}