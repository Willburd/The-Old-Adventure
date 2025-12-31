using System.Numerics;
using Engine;
using Rendering;

namespace EntComponents
{
    public class TestActorBehavior(Entity host_entity) : EntComponent(host_entity)
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        public override List<Core.Signals> DefaultSignals()
        {
            return [Core.Signals.load_assets, Core.Signals.create, Core.Signals.cache_components, Core.Signals.update];
        }

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

        protected override uint HandleCreate()
        {
            // Set the render's model and materials
            WorldRender? renderer = (WorldRender?)Host.GetComponent(typeof(WorldRender));
            renderer?.SetModel( AssetLoader.ModelAssetGet("sign"), AssetLoader.MaterialAssetGet("sign_wood"));
            renderer?.SetMaterial( AssetLoader.MaterialAssetGet("sign_face"), 1);
            
            return 1;
        }

        protected override uint HandleCacheComponents()
        {
            return 1;
        }

        protected override uint HandleUpdate()
        {
            
            Host.Rotation = Tools.LookAtLockedZ( Host.Position, Camera.WorldCamera.Position, Tools.Up);
            return 1;
        }
    }
}