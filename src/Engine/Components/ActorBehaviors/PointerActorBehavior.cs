using Engine;
using Assets;

namespace EntComponents.ActorBehavior
{
    public class PointerActorBehavior(Entity host_entity) : EntComponent(host_entity)
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.load_assets, Core.Signals.create, Core.Signals.update];
        }

        protected override uint HandleAssetLoad()
        {
            // Shaders
            ShaderData standard_shader = AssetLoader.ShaderAssetGet("standard", AssetLoader.AssetSource.engine);
            
            // Textures
            TextureData example_tex = AssetLoader.TextureAssetLoad( "example", AssetLoader.AssetDirectoryEngine + "/Textures/example.png", Silk.NET.OpenGL.TextureTarget.Texture2D, AssetLoader.AssetSource.engine);
            
            // Materials
            AssetLoader.MaterialAssetLoad( "example", new( [example_tex], [new("uTexture0", 0)], standard_shader), AssetLoader.AssetSource.engine);
            
            // Model
            AssetLoader.ModelAssetLoad( "test_pointer", AssetLoader.AssetDirectoryEngine + "/Models/pointer.fbx", AssetLoader.AssetSource.engine);

            return 1;
        }

        protected override uint HandleCreate()
        {
            // Set the render's model and materials
            WorldRender renderer = (WorldRender)Host.GetComponent(typeof(WorldRender));
            renderer.SetModel( AssetLoader.ModelAssetGet("test_pointer", AssetLoader.AssetSource.engine), AssetLoader.MaterialAssetGet("example", AssetLoader.AssetSource.engine));

            return 1;
        }

        protected override uint HandleUpdate()
        {
            if(Camera.WorldCamera != null)
            {
                Host.Rotation = Tools.LookAt( Host.Position, Camera.WorldCamera.Position);
            } 
            return 1;
        }
    }
}