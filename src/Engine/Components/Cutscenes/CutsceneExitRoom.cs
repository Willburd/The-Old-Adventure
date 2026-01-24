using Engine;
using System.Numerics;

namespace EntComponents.Cutscenes
{
    public class CutsceneExitRoom(Entity host_entity) : Cutscene(host_entity)
    {
        public Vector3 GoalPos { get; set; }

        private Vector3 dirvec = Vector3.Zero;

        public Room.RoomExit? ExitData = null;

        protected override uint HandleUpdate()
        {
            ActorBehavior.FadeoutActorBehavior fade_out = (ActorBehavior.FadeoutActorBehavior)Host.GetComponent(typeof(ActorBehavior.FadeoutActorBehavior));
            if (fade_out.FadeComplete)
            {
                // Trigger room transition
                if (ExitData != null)
                {
                    Host.Destroy();
                    Room new_room = (Room)Activator.CreateInstance(ExitData.Value.room_goal, [ExitData.Value.destination]);
                    ExitData = null;
                    ActorBehavior.FadeInActorBehavior fade_in = new ActorBehavior.FadeInActorBehavior(new_room);
                    fade_in.FadeColor = fade_out.FadeColor;
                }
            }

            return 1;
        }

        public override Vector3 Move()
        {
            // Get the direction we should go, and keep it
            if (dirvec == Vector3.Zero)
            {
                Actor player = Actor.GetPlayer();
                dirvec = Tools.FlatDirVector(player.Position, GoalPos);
            }
            return dirvec;
        }
    }
}