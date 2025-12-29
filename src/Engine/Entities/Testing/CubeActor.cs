using Engine;

namespace TestAdventure
{
    public class CubeActor : Actor
    {
        public CubeActor(Transform initial_location, Room? room_link = null) : base(initial_location, room_link)
        {
            new EntComponents.WorldRender(this);
            new EntComponents.CubeActorBehavior(this);
        }
    }
}