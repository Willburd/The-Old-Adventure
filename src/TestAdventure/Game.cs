TestGameEngine game_engine = new();

// Override of base game for testing
class TestGameEngine : Engine.Core
{
    public override void OnInit()
    {
        Console.WriteLine("START");
    }

    public override void OnEnd()
    {
        Console.WriteLine("END");
    }
}
