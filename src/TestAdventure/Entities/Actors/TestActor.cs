using Engine;

namespace TestAdventure
{
    public class TestActor : Actor
    {
        public TestActor(Transform initial_location, Room? room_link = null) : base(initial_location, room_link)
        {
            new EntComponents.TestActorBehavior(this);
        }
    }
}