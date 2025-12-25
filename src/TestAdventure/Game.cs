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
        public override void OnInit()
        {
            Console.WriteLine("START");
        }

        public override void OnLoadAssets()
        {
            AssetLoader.LoadAsset("test","path");
        }

        public override void OnEnd()
        {
            Console.WriteLine("END");
        }

        public override void OnGameTick()
        {
            Console.WriteLine("Tick " + ElapsedGameTicks);
        }
    }
}
