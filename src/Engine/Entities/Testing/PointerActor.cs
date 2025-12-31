using Engine;

namespace TestAdventure
{
    public class PointerActor : Actor
    {
        public PointerActor(Transform initial_location, Room? room_link = null) : base(initial_location, room_link)
        {
            new EntComponents.WorldRender(this);
            new EntComponents.PointerActorBehavior(this);
        }
    }
}