using System.Numerics;
using Engine;
using Rendering;

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
            model = AssetLoader.ModelAssetLoad( AssetLoader.AssetKey(Asset.AssetType.model, "test", AssetLoader.AssetSource.adventure) , AssetLoader.AssetDirectoryAdventure + "/Models/test.fbx"); // FBX 0.1 scale, Y up, X forward
            
            ShaderData test_shader = AssetLoader.ShaderAssetGet("standard",AssetLoader.AssetSource.engine);
            
            TextureData test_tex1 = AssetLoader.TextureAssetLoad( AssetLoader.AssetKey(Asset.AssetType.textures,"test1"), AssetLoader.AssetDirectoryAdventure + "/Textures/test1.png");
            TextureData test_tex2 = AssetLoader.TextureAssetLoad( AssetLoader.AssetKey(Asset.AssetType.textures,"test2"), AssetLoader.AssetDirectoryAdventure + "/Textures/test2.png");

            // Materials for each mesh in the model
            MaterialData shared_mat = new( [test_tex1], [new MaterialUniformData("uTexture0", 0)], test_shader);
            MaterialData alt_mat = new( [test_tex2], [new MaterialUniformData("uTexture0", 0)], test_shader);

            ApplyMaterial(AssetLoader.MaterialAssetGet( AssetLoader.AssetKey(Asset.AssetType.material, "debug_normals") ), model.Meshes.Count);
            
            return 1;
        }

        protected override uint HandleCreate()
        {
            WorldLocation? curloc = (WorldLocation?)Host.GetComponent(typeof(WorldLocation));
            
            curloc?.Position += new Vector3(0f,-0.5f,0f);
            curloc?.Rotation = Quaternion.CreateFromAxisAngle(Tools.Up, 15f);
            curloc?.Rotation = Quaternion.CreateFromAxisAngle(Tools.Forward, 75f);
            curloc?.Scale *= 1f;
            curloc?.SnapTransform();

            return 1;
        }

        protected override uint HandleUpdate()
        {
            WorldLocation? curloc = (WorldLocation?)Host.GetComponent(typeof(WorldLocation));
            curloc?.Rotation *= Quaternion.CreateFromAxisAngle(Tools.Up, 0.02f);
            curloc?.SnapTransform();

            return 1;
        }
    }
}