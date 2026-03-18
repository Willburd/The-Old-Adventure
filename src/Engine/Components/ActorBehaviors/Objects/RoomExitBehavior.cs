using Engine;
using System.Numerics;

namespace EntComponents.ActorBehavior
{
    public class RoomExitBehavior(Entity host_entity) : EntComponent(host_entity)
    {
        public int ExitID { get; set; } = 0;
        public Vector2 Size
        {
            get
            {
                TriggerVolume trigger = (TriggerVolume)Host.GetComponent(typeof(TriggerVolume));
                Engine.ColliderShapes.CylinderCol cyl = (Engine.ColliderShapes.CylinderCol)trigger.CollisionShape;
                return new(cyl.radius, cyl.height);
            }

            set
            {
                TriggerVolume trigger = (TriggerVolume)Host.GetComponent(typeof(TriggerVolume));
                trigger.CollisionMask = Collider.mask_player;
                trigger.SetShape(new Engine.ColliderShapes.CylinderCol(value.X, value.Y));
            }
        }

        public Room.RoomExit ExitDestination { get; set; }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.trigger_start];
        }

        protected override uint HandleTriggerStart(Collider new_collision)
        {
            Entity colliding = new_collision.Host;
            if (colliding.EntityID == PlayerActorBehavior.player_actor_id)
            {
                Entity.SendGlobalSignal(Core.Signals.global_room_exit_trigger, ExitDestination, Host);
                if (Actor.OwnerRoom.OnUseExit(ExitDestination, Host)) return 1;
            }
            return 0;
        }
    }
}