using Engine;

namespace TestAdventure
{
    public class TestActor : Actor
    {
        public TestActor(Room? room_link) : base(room_link)
        {
            new EntComponents.WorldLocation(this);
            new EntComponents.TestActorBehavior(this);
        }
    }
}