using System.Numerics;
using Engine;
using Rendering;
using Silk.NET.Assimp;

namespace EntComponents
{
    public class TestActorBehavior(Entity host_entity) : Renders(host_entity)
    {
        public override List<Core.Signals> DefaultSignals()
        {
            return [Core.Signals.asset_load, Core.Signals.create, Core.Signals.cache_components, Core.Signals.update, Core.Signals.render_priority, Core.Signals.render];
        }

        protected override uint HandleAssetLoad()
        {
            AssetLoader.ModelAssetLoad("model_test", Tools.AssetDirectory + "/Models/test.obj");
            ShaderData test_shader = AssetLoader.ShaderAssetLoad("shader_test", Tools.AssetDirectory + "/Shaders/test");
            TextureData test_tex = AssetLoader.TextureAssetLoad("texture_test", Tools.AssetDirectory + "/Textures/test.png");

            // Materials for each mesh in the model
            MaterialData shared_mat = new( [test_tex], [new MaterialUniformData("uTexture0", 0)], test_shader);
            materials.Add(shared_mat);
            materials.Add(shared_mat);
            materials.Add(shared_mat);

            // Load the model with the materials we assigned
            model = AssetLoader.ModelAssetGet("model_test");

            return 1;
        }

        protected override uint HandleCreate()
        {
            WorldLocation? curloc = (WorldLocation?)Host.GetComponent(typeof(WorldLocation));
            
            curloc?.Rotation = Quaternion.CreateFromAxisAngle(Tools.Up, 15f);
            curloc?.Scale *= 6;
            curloc?.SnapTransform();

            return 1;
        }

        protected override uint HandleUpdate()
        {
            WorldLocation? curloc = (WorldLocation?)Host.GetComponent(typeof(WorldLocation));
            curloc?.Rotation *= Quaternion.CreateFromAxisAngle(Tools.Up, 0.1f);
            curloc?.SnapTransform();

            return 1;
        }
    }
}