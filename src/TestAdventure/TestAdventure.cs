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
            AssetLoader.TextureAssetLoad( "overworld_skybox_dawn", AssetLoader.AssetDirectoryAdventure + "/Textures/skybox_dawn.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            AssetLoader.TextureAssetLoad( "overworld_skybox_day", AssetLoader.AssetDirectoryAdventure + "/Textures/skybox_day.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            AssetLoader.TextureAssetLoad( "overworld_skybox_dusk", AssetLoader.AssetDirectoryAdventure + "/Textures/skybox_dusk.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            AssetLoader.TextureAssetLoad( "overworld_skybox_night", AssetLoader.AssetDirectoryAdventure + "/Textures/skybox_night.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
        }

        public override void OnPreInit()
        {
            // Sets engine to use our entity factory and all the entities we've made unique to our adventure.
            EntityFactory.entity_Factory = new TestAdventureEntityFactory();
        }

        public override void OnInit()
        {
            // Start up initial room
            new TestRoom();
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
