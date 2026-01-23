using Engine;

namespace TestAdventure
{
    public class Program
    {
        public static void Main(string[] args)
        {
            // Start the engine
            new TestGameEngine("TestAdventure");
        }
    }

    // Override of base game for testing
    class TestGameEngine(string ID) : Core(ID)
    {
        public override void OnLoadAssets()
        {
            // Load our global assets
        }

        public override void OnPreInit()
        {
            
        }

        public override void OnInit()
        {
            // Start up initial room
            new TestRoom("room_testing", Room.EntranceType.debug);
        }

        public override void OnNewGame()
        {
            // Setup the initial savestate
            Savestate.ResetState();
            Savestate.SetValue(Savestate.player_max_health, 4 * 20);
            Savestate.SetValue(Savestate.player_health, 4 * 3);
        }

        public override void OnGameTick()
        {
            // Handle global tick events

        }

        public override void OnEnd()
        {
            // Cleanup during shutdown

        }
    }
}
