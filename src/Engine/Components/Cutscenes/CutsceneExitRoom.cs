using Engine;
using System.Numerics;

namespace EntComponents.Cutscenes
{
    public class CutsceneExitRoom : Cutscene
    {
        public Vector3 GoalPos { get; set; }

        private Vector3 dirvec = Vector3.Zero;

        public Room.RoomExit? ExitData = null;

        public CutsceneExitRoom(Entity host_entity) : base(host_entity)
        {
            // Add a fade out so we have one for our fade in later
            new ActorBehavior.FadeoutActorBehavior(host_entity);
        }

        protected override uint HandleUpdate()
        {
            // Trigger room transition
            if (ExitData != null)
            {
                ActorBehavior.FadeoutActorBehavior fade_out = (ActorBehavior.FadeoutActorBehavior)Host.GetComponent(typeof(ActorBehavior.FadeoutActorBehavior));
                if (fade_out.FadeComplete)
                {
                    Room new_room = (Room)Activator.CreateInstance(ExitData.Value.room_goal, [ExitData.Value.destination]);
                    ExitData = null;
                    ActorBehavior.FadeInActorBehavior fade_in = new(new_room)
                    {
                        FadeColor = fade_out.FadeColor
                    };
                    Host.Destroy();
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