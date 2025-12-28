using System.Numerics;
using Engine;
using Rendering;

namespace EntComponents
{
    public class TestActorBehavior(Entity host_entity) : Renders(host_entity)
    {
        protected override uint HandleAssetLoad()
        {
            // Shaders
            ShaderData standard_shader = AssetLoader.ShaderAssetGet("standard", AssetLoader.AssetSource.engine);
            
            // Textures
            TextureData sign_wood = AssetLoader.TextureAssetLoad( "sign_wood", AssetLoader.AssetDirectoryAdventure + "/Textures/sign_wood.png");
            TextureData sign_face = AssetLoader.TextureAssetLoad( "sign_face", AssetLoader.AssetDirectoryAdventure + "/Textures/sign_face.png");
            
            // Materials
            AssetLoader.MaterialAssetLoad( "sign_wood", new( [sign_wood], [new MaterialUniformData("uTexture0", 0)], standard_shader));
            AssetLoader.MaterialAssetLoad( "sign_face", new( [sign_face], [new MaterialUniformData("uTexture0", 0)], standard_shader));
            
            // Model
            AssetLoader.ModelAssetLoad( "sign", AssetLoader.AssetDirectoryAdventure + "/Models/sign.fbx");

            return 1;
        }

        float spin_speed = 0;

        protected override uint HandleCreate()
        {
            // Apply assets
            model = AssetLoader.ModelAssetGet("sign");
            materials.Add(AssetLoader.MaterialAssetGet( "sign_wood")); // sign
            materials.Add(AssetLoader.MaterialAssetGet( "sign_face")); // face
            
            // Set location
            Host.Scale *= Tools.RandRange(0.5f,1.5f);
            Host.SnapTransform();

            spin_speed = Tools.RandRange(0.1f,0.6f);

            return 1;
        }

        protected override uint HandleUpdate()
        {
            Host.Rotation *= Quaternion.CreateFromAxisAngle(Tools.Up, spin_speed);
            return 1;
        }
    }
}