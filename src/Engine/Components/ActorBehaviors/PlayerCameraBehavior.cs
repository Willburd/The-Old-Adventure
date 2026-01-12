using Engine;
using System.Numerics;

namespace EntComponents.ActorBehavior
{
    public class PlayerCameraBehavior(Entity host_entity) : EntComponent(host_entity)
    {
        private float distance_from_player = 4.4f;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.create, Core.Signals.update];
        }

        protected override uint HandleUpdate()
        {
            // Influence camera movement
            Input? input = (Input?)Host.GetComponent(typeof(Input));
            if(input != null)
            {
                Vector3 current_pos = Host.Position;
                //current_pos += Vector3.Transform( Tools.Right, Quaternion.CreateFromAxisAngle( Vector3.Transform(Tools.Up, Quaternion.Inverse(Host.Rotation)), input.CameraInput.X * InputHandler.mouse_sensitivity));
                //current_pos += Vector3.Transform( Tools.Up, Quaternion.CreateFromAxisAngle( Tools.Right, input.CameraInput.Y * InputHandler.mouse_sensitivity));
                Host.Position = current_pos;
            }

            // Try to orbit player
            Actor? player = Actor.GetActor( PlayerActorBehavior.player_actor_id);
            if(player == null) return 0;

            // From the center of the player
            Vector3 start_pos = player.Position + Tools.Up; // halfway up actor, more or less.
            if(Host.Position == player.Position) Host.Position += Vector3.Transform(Tools.Backward * 0.01f, player.Rotation); // no trapping it
            
            // Get our desired location, then check if we're blocked by a wall
            Vector3 want_offset = Tools.DirVector(start_pos, Host.Position) * distance_from_player;
            Vector3 goal_pos = start_pos + want_offset;
            Collider.RaycastHit? hit = Collider.DoRaycastNearest(start_pos, want_offset, Collider.mask_worldgeo);
            if(hit != null)
            {
                goal_pos = hit.Value.HitPosition;
            }

            // Move toward our goal
            Host.Position = Vector3.Lerp( Host.Position, goal_pos, 0.8f) ;
            Host.Rotation = Tools.LookAt(Host.Position, start_pos);
            return 1;
        }
    }
}