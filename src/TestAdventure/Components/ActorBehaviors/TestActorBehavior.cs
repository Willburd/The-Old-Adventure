using System.Numerics;
using Engine;
using Assets;

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
            TextureData sign_wood = AssetLoader.TextureAssetLoad("sign_wood", AssetLoader.AssetDirectoryAdventure + "/Textures/sign_wood.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            TextureData sign_face = AssetLoader.TextureAssetLoad("sign_face", AssetLoader.AssetDirectoryAdventure + "/Textures/sign_face.png", Silk.NET.OpenGL.TextureTarget.Texture2D);

            // Materials
            AssetLoader.MaterialAssetLoad("sign_wood", new([sign_wood], [new("uTexture0", 0)], standard_shader));
            AssetLoader.MaterialAssetLoad("sign_face", new([sign_face], [new("uTexture0", 0)], standard_shader));

            // Model
            AssetLoader.ModelAssetLoad("sign", AssetLoader.AssetDirectoryAdventure + "/Models/sign.fbx");

            return 1;
        }

        protected override uint HandleCreate()
        {
            // Set the render's model and materials
            WorldRender renderer = (WorldRender)Host.GetComponent(typeof(WorldRender));

            var sign_model = AssetLoader.ModelAssetGet("sign");
            renderer.SetModel(sign_model, AssetLoader.MaterialAssetGet("sign_wood"));
            renderer.SetMaterial(AssetLoader.MaterialAssetGet("sign_face"), sign_model.GetMeshIndex("Sign_1"));

            // Collision
            Collider collision = (Collider?)Host.GetComponent(typeof(Collider));
            collision.SetShape(new Engine.ColliderShapes.PointCol());

            // Trigger
            TriggerVolume trigger = (TriggerVolume?)Host.GetComponent(typeof(TriggerVolume));
            trigger.SetShape(new Engine.ColliderShapes.SphereCol(0.5f));
            trigger.OffsetPos = new(0f, 0f, 1f);

            return 1;
        }

        protected override uint HandleUpdate()
        {
            Host.Rotation *= Quaternion.CreateFromAxisAngle(Tools.Up, 0.01f);

            return 1;
        }
    }
}