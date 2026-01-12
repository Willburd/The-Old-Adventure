
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
            Input? input = (Input?)Host.GetComponent(typeof(Input));
            PhysicsBody? phys = (PhysicsBody?)GetComponent(typeof(PhysicsBody));
            Collider.RaycastHit? hit = Collider.DoRaycastNearest(Host.Position + Tools.Up, Tools.Down * 1.1f, Collider.mask_worldgeo);
            if(hit != null && phys != null)
            {
                // Snap to floor
                Host.Position = new Vector3(Host.Position.X, (hit.Value.HitPosition + (Tools.Up * 0.01f)).Y, Host.Position.Z);
                phys.HasGravity = false;
                phys.Velocity = new Vector3(phys.Velocity.X, 0f, phys.Velocity.Z);

                // Movement
                if(input != null)
                {
                    // Rotation
                    Vector3 move_dir = input.MoveInput;
                    Console.WriteLine(move_dir);
                    Host.Rotation *= Tools.CreateFromAxisAngle(Tools.Up, move_dir.X * -0.1f);
                    // Movement
                    move_dir.X = 0f;
                    float player_move_speed = 0.02f;
                    phys.Velocity += Vector3.Transform(move_dir, Host.Rotation) * player_move_speed;
                }
            }
            else
            {
                // Gravity
                phys?.HasGravity = true;
            }
        }
    }
}