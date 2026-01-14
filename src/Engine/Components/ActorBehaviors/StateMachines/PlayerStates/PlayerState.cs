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
        protected const float air_turnrate = 0.8f;
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

        private Collider.RaycastHit? FloorCollision()
        {
            Collider.RaycastHit? hit = Collider.DoRaycastNearest(Host.Position + Tools.Up, Tools.Down * (1f + ground_snap_distance), Collider.mask_worldgeo);
            if (hit != null && !hit.Value.IsCeil)
            {
                Host.Position = new Vector3(hit.Value.HitPosition.X, hit.Value.HitPosition.Y, hit.Value.HitPosition.Z) + new Vector3(0f, -ground_snap_distance, 0f);
                return hit;
            }
            return null;
        }

        private Collider.RaycastHit? WallCollision(Vector3 velocity, float height, float radius)
        {
            velocity.Y = 0f; // Flatten it
            velocity = Vector3.Normalize(velocity);

            Collider.RaycastHit? hit = Collider.DoRaycastNearest(Host.Position + (Tools.Up * height), velocity * radius, Collider.mask_worldgeo);
            if (hit != null && !hit.Value.IsFloor)
            {
                Host.Position = hit.Value.HitPosition + (velocity * -radius) + (Tools.Down * height);
                return hit;
            }
            return null;
        }

        private Collider.RaycastHit? CeilingCollision(float height)
        {
            Collider.RaycastHit? hit = Collider.DoRaycastNearest(Host.Position, Tools.Up * height, Collider.mask_worldgeo);
            if (hit != null && hit.Value.IsCeil)
            {
                Host.Position = hit.Value.HitPosition + (Tools.Down * height);
                return hit;
            }
            return null;
        }

        protected Collider.RaycastHit? StandardProcessFloors(PhysicsBody phys)
        {
            // Snap to floor
            Collider.RaycastHit? hit = FloorCollision();
            if (hit != null)
            {
                Host.Position = new Vector3(hit.Value.HitPosition.X, hit.Value.HitPosition.Y, hit.Value.HitPosition.Z) + new Vector3(0f, -ground_snap_distance, 0f);
                phys.Velocity = new Vector3(phys.Velocity.X, 0f, phys.Velocity.Z);
                return hit;
            }
            return null;
        }

        protected Collider.RaycastHit? StandardProcessWalls(PhysicsBody phys, float player_radius, float height)
        {
            // Check all walls around us in all directions, ensure we are not intruding into them
            Collider.RaycastHit? nearest_hit = null;
            for (int i = 0; i < 360; i += 45)
            {
                // Check upper Wallblocking
                Vector3 ray_dir = Vector3.Transform(Tools.Forward, Quaternion.CreateFromAxisAngle(Tools.Up, i));
                Collider.RaycastHit? upper_hit = WallCollision(ray_dir, height - wallcast_y_lower, player_radius);
                Collider.RaycastHit? lower_hit = WallCollision(ray_dir, wallcast_y_lower, player_radius); // secondary check
                
                // Check for furthest hit on upper and lower (we want to always eject as much as we possibly can from a wall)
                Collider.RaycastHit? considered_hit = null;
                if (lower_hit != null && !lower_hit.Value.IsFloor && lower_hit.Value.Distance > upper_hit?.Distance) considered_hit = lower_hit;
                if (upper_hit != null && !upper_hit.Value.IsFloor && upper_hit.Value.Distance > lower_hit?.Distance) considered_hit = upper_hit;
                if (considered_hit != null && considered_hit.Value.Distance > nearest_hit?.Distance) nearest_hit = considered_hit;
            }
            // Finally apply pushout on the worst offender
            if (nearest_hit != null)
            {
                Host.Position -= Vector3.Normalize(nearest_hit.Value.Direction) * nearest_hit.Value.Distance;
                phys.Velocity = new Vector3(0f, phys.Velocity.Y, 0f);
            }
            return nearest_hit;
        }

        protected Collider.RaycastHit? StandardProcessCeilings(PhysicsBody phys, float height)
        {
            // Check for ceiling, prevent movement into crushing spaced
            Collider.RaycastHit? ceilhit = CeilingCollision(height);
            if (ceilhit != null)
            {
                phys.Velocity = new Vector3(0f, MathF.Min(0f, phys.Velocity.Y), 0f);
                return ceilhit;
            }
            return null;
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