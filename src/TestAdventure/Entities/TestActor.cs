namespace Engine
{
    public class TestActor : Actor
    {
        public TestActor() : base()
        {
            new EntComponents.WorldLocation(this);
            new EntComponents.TestActorBehavior(this);
        }
    }
}