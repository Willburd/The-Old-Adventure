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
        protected const float ground_friction = 0.009f;
        protected const float ground_run_maxspeed = 0.1f;

        // Air
        protected const float air_acceleration = 0.018f;
        protected const float air_friction = 0.004f;
        protected const float air_maxspeed = 0.08f;
        
        // Slipping
        protected const float slip_threshold = 0.03f;
        protected const float slip_acceleration = 0.056f;
        protected const float slip_maxspeed = 0.15f;
        
        // Collision raycasts
        protected const float ground_snap_distance = 0.09f; // Amount above the ground that the origin of the player will be
        protected const float wallcast_y_lower = 0.04f; // Beneath this point walls will be ignored (like steps on a staircase)
        protected const float character_height = 0.65f; // The height of the character, preventing movement under ceilings

        protected PlayerActorBehavior Player
        {
            get
            {
                return (PlayerActorBehavior)Owner;
            }
        }

        protected Quaternion CameraRotationToPlayer()
        {
            Vector3 campos = Camera.WorldCamera?.Position ?? Vector3.Zero;
            return Tools.FlatRotation(Tools.DirVector(campos, Host.Position));
        }

        protected Collider.RaycastHit? FloorCollision()
        {
            Collider.RaycastHit? hit = Collider.DoRaycastNearest(Host.Position + Tools.Up, Tools.Down * (1f + ground_snap_distance), Collider.mask_worldgeo);
            if (hit != null) Host.Position = new Vector3(hit.Value.HitPosition.X, hit.Value.HitPosition.Y, hit.Value.HitPosition.Z) + new Vector3(0f, -ground_snap_distance, 0f);
            return hit;
        }

        protected Collider.RaycastHit? WallCollision(Vector3 velocity, float height, float radius)
        {
            Vector3 moving_vector = velocity;
            moving_vector.Y = 0f; // Flatten it
            moving_vector = Vector3.Normalize(moving_vector);

            Collider.RaycastHit? hit = Collider.DoRaycastNearest(Host.Position + (Tools.Up * height), moving_vector * radius, Collider.mask_worldgeo);
            if (hit != null && hit.Value.IsWall) Host.Position = hit.Value.HitPosition + (Tools.Down * height) + (moving_vector * -radius);
            return hit;
        }

        protected void StandardProcessWalls(PhysicsBody phys, float player_radius)
        {
            // Check upper Wallblocking
            Collider.RaycastHit? velocity_hit = WallCollision(phys.Velocity, character_height, player_radius);
            velocity_hit ??= WallCollision(phys.Velocity, wallcast_y_lower, player_radius); // secondary check
            if (velocity_hit == null) return;
            phys.Velocity = new Vector3(0f, phys.Velocity.Y, 0f);
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