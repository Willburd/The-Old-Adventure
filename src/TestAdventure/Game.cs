using Engine;

TestGameEngine game_engine = new();

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
}
