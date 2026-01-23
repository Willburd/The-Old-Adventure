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
            ActorBehavior.FadeoutActorBehavior fade = (ActorBehavior.FadeoutActorBehavior)Host.GetComponent(typeof(ActorBehavior.FadeoutActorBehavior));
            if (fade.FadeComplete)
            {
                // Trigger room transition
                if (ExitData != null)
                {
                    Host.Destroy();
                    Activator.CreateInstance(ExitData.Value.room_goal, [ExitData.Value.destination]);
                    ExitData = null;
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