using Engine;

namespace TestAdventure
{
    public class Program
    {
        public static void Main(string[] args) 
        { 
            // Start the engine
            new TestGameEngine();
        }
    }

    // Override of base game for testing
    class TestGameEngine : Core
    {
        public override void OnLoadAssets()
        {
            // Load assets with the assetloader
            base.OnLoadAssets();

        }

        public override void OnInit()
        {
            // Start up initial scene
            base.OnInit();
            new TestActor();
        }

        public override void OnGameTick()
        {
            // Handle global tick events
            base.OnGameTick();

        }

        public override void OnEnd()
        {
            // Cleanup during shutdown
            base.OnEnd();
        }
    }
}
