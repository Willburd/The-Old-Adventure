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
            AssetLoader.ModelAssetLoad("model_test", AssetLoader.AssetDirectoryAdventure + "/Models/cube.model");
            ShaderData test_shader = AssetLoader.ShaderAssetLoad("shader_test", AssetLoader.AssetDirectoryAdventure + "/Shaders/test");
            TextureData test_tex = AssetLoader.TextureAssetLoad("texture_test", AssetLoader.AssetDirectoryAdventure + "/Textures/test.png");

            // Materials for each mesh in the model
            materials.Add( new( [test_tex], [new MaterialUniformData("uTexture0", 0)], test_shader));
            
            // Load the model with the materials we assigned
            model = AssetLoader.ModelAssetGet("model_test");

            return 1;
        }

        protected override uint HandleCreate()
        {
            WorldLocation? curloc = (WorldLocation?)Host.GetComponent(typeof(WorldLocation));
            
            curloc?.Rotation = Quaternion.CreateFromAxisAngle(Tools.Up, 15f);
            curloc?.Rotation = Quaternion.CreateFromAxisAngle(Tools.Forward, 75f);
            curloc?.Scale *= 0.6f;
            curloc?.SnapTransform();

            return 1;
        }

        protected override uint HandleUpdate()
        {
            WorldLocation? curloc = (WorldLocation?)Host.GetComponent(typeof(WorldLocation));
            curloc?.Rotation *= Quaternion.CreateFromAxisAngle(Tools.Up, 0.04f);
            curloc?.SnapTransform();

            return 1;
        }
    }
}