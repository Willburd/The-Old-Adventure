using System.Numerics;
using Engine;
using Rendering;

namespace EntComponents
{
    public class CubeActorBehavior(Entity host_entity) : EntComponent(host_entity)
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
            AssetLoader.ModelAssetLoad( "test_cube", AssetLoader.AssetDirectoryEngine + "/Models/cube.obj", AssetLoader.AssetSource.engine);

            return 1;
        }

        protected override uint HandleCreate()
        {
            // Set the render's model and materials
            WorldRender? renderer = (WorldRender?)Host.GetComponent(typeof(WorldRender));
            renderer?.SetModel( AssetLoader.ModelAssetGet("test_cube", AssetLoader.AssetSource.engine), AssetLoader.MaterialAssetGet("example", AssetLoader.AssetSource.engine));
            Host.MoveTransform(new Transform(new Vector3(0.4f,0f,0f), Tools.Euler(-45f,0f,0f), new Vector3(-0.5f,-0.5f,-0.5f)));

            return 1;
        }

        protected override uint HandleCacheComponents()
        {
            return 1;
        }

        protected override uint HandleUpdate()
        {
            Host.Rotation *= Tools.Euler(0f,1f,0f);
            return 1;
        }
    }
}