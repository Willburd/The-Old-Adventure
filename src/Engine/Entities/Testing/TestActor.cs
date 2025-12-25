namespace Engine
{
    public class TestActor : Actor
    {
        public TestActor() : base()
        {
            new EntComponents.TestRender(this);
        }
    }
}