using Engine;

namespace TestAdventure
{
    public class Program
    {
        public static void Main(string[] args) 
        { 
            TestGameEngine game_engine = new();
        }
    }

    // Override of base game for testing
    class TestGameEngine : Core
    {
        public override void OnLoadAssets()
        {
            AssetLoader.LoadAsset("test","path");
        }

        public override void OnInit()
        {
            Console.WriteLine("START");
            new TestActor();
        }

        public override void OnGameTick()
        {
            
        }

        public override void OnEnd()
        {
            Console.WriteLine("END");
        }
    }
}
