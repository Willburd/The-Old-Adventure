using System.Numerics;
using Engine;
using Rendering;

namespace EntComponents
{
    public class PointerActorBehavior(Entity host_entity) : EntComponent(host_entity)
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
            TextureData example_tex = AssetLoader.TextureAssetLoad( "example", AssetLoader.AssetDirectoryEngine + "/Textures/example.png", AssetLoader.AssetSource.engine);
            
            // Materials
            AssetLoader.MaterialAssetLoad( "example", new( [example_tex], [new MaterialUniformData("uTexture0", 0)], standard_shader), AssetLoader.AssetSource.engine);
            
            // Model
            AssetLoader.ModelAssetLoad( "test_pointer", AssetLoader.AssetDirectoryEngine + "/Models/pointer.fbx", AssetLoader.AssetSource.engine);

            return 1;
        }

        protected override uint HandleCreate()
        {
            // Set the render's model and materials
            WorldRender? renderer = (WorldRender?)Host.GetComponent(typeof(WorldRender));
            renderer?.SetModel( AssetLoader.ModelAssetGet("test_pointer", AssetLoader.AssetSource.engine), AssetLoader.MaterialAssetGet("example", AssetLoader.AssetSource.engine));

            return 1;
        }

        protected override uint HandleCacheComponents()
        {
            return 1;
        }

        protected override uint HandleUpdate()
        {
            Host.Rotation = Tools.LookAtLockedZ( Host.Position, Camera.WorldCamera.Position);
            Console.WriteLine( Camera.WorldCamera.Position +  "  > "  + Vector3.Transform(Tools.Forward, Host.Rotation));
            return 1;
        }
    }
}