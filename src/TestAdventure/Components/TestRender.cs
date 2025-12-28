using System.Numerics;
using Engine;
using Rendering;

namespace EntComponents
{
    public class TestActorBehavior(Entity host_entity) : Renders(host_entity)
    {
        public override List<Core.Signals> DefaultSignals()
        {
            return [Core.Signals.create, Core.Signals.cache_components, Core.Signals.update, Core.Signals.render_priority, Core.Signals.render];
        }

        protected override uint HandleCreate()
        {
            // Apply assets
            model                           = AssetLoader.ModelAssetGet(    AssetLoader.AssetKey(Asset.AssetType.model,    "cube",    AssetLoader.AssetSource.engine));
            MaterialData default_material   = AssetLoader.MaterialAssetGet( AssetLoader.AssetKey(Asset.AssetType.material, "example", AssetLoader.AssetSource.engine));
            ApplyMaterial( default_material, model.Meshes.Count);
            
            // Set location
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