using Engine;
using Rendering;

namespace EntComponents.ActorBehavior
{
    public class TestActorBehavior(Entity host_entity) : EntComponent(host_entity)
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

            var sign_model = AssetLoader.ModelAssetGet("sign");
            renderer?.SetModel( sign_model, AssetLoader.MaterialAssetGet("sign_wood"));
            renderer?.SetMaterial( AssetLoader.MaterialAssetGet("sign_face"), sign_model.GetMeshIndex("Sign_1"));
            
            // Collision
            Collider? collision = (Collider?)Host.GetComponent(typeof(Collider));
            collision?.SetShape( new Engine.ColliderShapes.CylinderCol(0.6f,0.25f));

            // Trigger
            TriggerVolume? trigger = (TriggerVolume?)Host.GetComponent(typeof(TriggerVolume));
            trigger?.SetShape( new Engine.ColliderShapes.CylinderCol(0.3f,0.5f));

            return 1;
        }

        protected override uint HandleUpdate()
        {
            return 1;
        }
    }
}