using Engine;
using Rendering;

namespace TestAdventure
{
    public class TestRoom : Room
    {
        public override void LoadAssets()
        {
            // Shaders (You should preload these in the adventurer's Core and only get the asset here)
            ShaderData standard_shader = AssetLoader.ShaderAssetGet("standard", AssetLoader.AssetSource.engine);

            // Textures
            TextureData sign_wood = AssetLoader.TextureAssetLoad( AssetLoader.AssetKey(Asset.AssetType.textures, "sign_wood"), 
                                                                    AssetLoader.AssetDirectoryAdventure + "/Textures/sign_wood.png");
            TextureData sign_face = AssetLoader.TextureAssetLoad( AssetLoader.AssetKey(Asset.AssetType.textures, "sign_face"), 
                                                                    AssetLoader.AssetDirectoryAdventure + "/Textures/sign_face.png");

            // Materials
            AssetLoader.MaterialAssetLoad( AssetLoader.AssetKey(Asset.AssetType.material, "sign_wood"),
                                            new( [sign_wood], [new MaterialUniformData("uTexture0", 0)], standard_shader));
            AssetLoader.MaterialAssetLoad( AssetLoader.AssetKey(Asset.AssetType.material, "sign_face"),
                                            new( [sign_face], [new MaterialUniformData("uTexture0", 0)], standard_shader));

            // Models
            AssetLoader.ModelAssetLoad( AssetLoader.AssetKey(Asset.AssetType.model,"sign"), 
                                            AssetLoader.AssetDirectoryAdventure + "/Models/sign.fbx");

        }

        public override void LoadActors()
        {
            new TestActor(this);
        }

        public override void OnRoomUpdate()
        {
            // Test unload
            if(Core.ElapsedGameTicks >= 300) Destroy();
        }
    }
}