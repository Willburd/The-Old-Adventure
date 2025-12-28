using Engine;
using Rendering;
using System.Numerics;

namespace TestAdventure
{
    public class TestRoom(Transform initial_location) : Room(initial_location)
    {
        public override void LoadAssets()
        {
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Shaders (You should preload these in the adventurer's Core and only get the asset here)
            ShaderData standard_shader = AssetLoader.ShaderAssetGet("standard", AssetLoader.AssetSource.engine);

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Textures
            TextureData sign_wood = AssetLoader.TextureAssetLoad( "sign_wood", AssetLoader.AssetDirectoryAdventure + "/Textures/sign_wood.png");
            TextureData sign_face = AssetLoader.TextureAssetLoad( "sign_face", AssetLoader.AssetDirectoryAdventure + "/Textures/sign_face.png");

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Materials
            AssetLoader.MaterialAssetLoad( "sign_wood", new( [sign_wood], [new MaterialUniformData("uTexture0", 0)], standard_shader));
            AssetLoader.MaterialAssetLoad( "sign_face", new( [sign_face], [new MaterialUniformData("uTexture0", 0)], standard_shader));

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Models
            AssetLoader.ModelAssetLoad( "sign", AssetLoader.AssetDirectoryAdventure + "/Models/sign.fbx");
        }

        public override void LoadActors()
        {
            float spread = 0.8f;
            new TestActor(Transform.Identity, this);
            new TestActor(new Transform(new Vector3(Tools.RandRange(-spread,spread),-0.5f + Tools.RandRange(-spread,spread),Tools.RandRange(-spread,spread))), this);
            new TestActor(new Transform(new Vector3(Tools.RandRange(-spread,spread),-0.5f + Tools.RandRange(-spread,spread),Tools.RandRange(-spread,spread))), this);
        }

        public override void OnRoomUpdate()
        {
            
        }
    }
}