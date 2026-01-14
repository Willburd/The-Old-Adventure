using System.Numerics;
using Engine;
using Engine.ColliderShapes;

namespace EntComponents.ActorBehavior.PlayerStates
{
    public class PlayerState(PlayerActorBehavior owner) : BehaviorStateMachine(owner)
    {
        // Speeds
        protected const float ground_turnrate = 0.12f;
        protected const float ground_acceleration = 0.022f;
        protected const float ground_friction = 0.014f;
        protected const float ground_run_maxspeed = 0.1f;
        protected const float slip_acceleration = 0.012f;
        protected const float slip_maxspeed = 0.15f;
        
        // Collision raycasts
        protected const float ground_snap_distance = 0.01f; // Amount above the ground that the origin of the player will be
        protected const float wallcast_y_lower = 0.1f; // Beneath this point walls will be ignored (like steps on a staircase)
        protected const float character_height = 0.65f; // The height of the character, preventing movement under ceilings

        protected Quaternion CameraRotationToPlayer()
        {
            Vector3 campos = Camera.WorldCamera?.Position ?? Vector3.Zero;
            return Tools.FlatRotation(Tools.DirVector(campos, Host.Position));
        }

        protected Collider.RaycastHit? WallCollision(Vector3 velocity, float height, float radius)
        {
            float org_y = Host.Position.Y;
            Vector3 moving_vector = velocity;
            moving_vector.Y = 0f; // Flatten it
            moving_vector = Vector3.Normalize(moving_vector);

            Collider.RaycastHit? hit = Collider.DoRaycastNearest(Host.Position + (Tools.Up * height), moving_vector * radius, Collider.mask_worldgeo);
            if (hit != null)
            {
                // Get the position we'd arrive at when we hit the wall
                float remaining_distance = hit.Value.Distance - radius;
                Host.Position = Host.Position + (moving_vector * remaining_distance);
                Host.Position += new Vector3(0f, org_y, 0f);
            }
            return hit;
        }

        protected float FloorSlipFactor(Vector3 normal)
        {
            if(normal.Y > WorldGeometryCol.wall_y_threshold)
            {
                // Floor
                return 1f - normal.Y;
            }
            else
            {
                // wall or ceiling
                return 1f;
            }
        }
    }
}