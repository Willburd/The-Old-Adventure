using Engine;
using System.Numerics;

namespace EntComponents.ActorBehavior.CameraStates
{
    public class CameraOrbitFollow(PlayerCameraBehavior owner) : CameraState(owner)
    {
        private float camera_rotation_angle = 0f;
        private float camera_pitch_angle = 3.26f;

        public override void Process()
        {
            // Try to orbit player
            Actor? player = Actor.GetActor( PlayerActorBehavior.player_actor_id);
            if(player == null) return;

            // From the center of the player
            Vector3 start_pos = player.Position + (Tools.Up * 0.2f); // halfway up actor, more or less.
            
            // Camera movement
            Input? input = (Input?)Host.GetComponent(typeof(Input));
            if(input != null)
            {
                camera_rotation_angle += input.CameraInput.X * 0.0005f;
                camera_pitch_angle += input.CameraInput.Y * 0.0005f;
                camera_pitch_angle = Math.Clamp(camera_pitch_angle,2f,4.50f);
            }

            // Get our desired location, then check if we're blocked by a wall
            Quaternion aim_rotation = Quaternion.Inverse(Quaternion.CreateFromAxisAngle(Tools.Right, camera_pitch_angle) * Quaternion.CreateFromAxisAngle(Tools.Up, camera_rotation_angle));
            Vector3 want_offset = Vector3.Transform(Tools.Forward, aim_rotation) * distance_from_player;

            // Push against walls
            Vector3 goal_pos = start_pos + want_offset;
            Collider.RaycastHit? hit = Collider.DoRaycastNearest(start_pos, want_offset, Collider.mask_worldgeo);
            if(hit != null)
            {
                //goal_pos = hit.Value.HitPosition;
            }

            // Move to the new position
            Host.Position = goal_pos;
            Host.Rotation = Tools.LookAt(Host.Position, start_pos);
        }
    }
}