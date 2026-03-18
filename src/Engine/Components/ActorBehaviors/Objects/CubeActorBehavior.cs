using Engine;
using Assets;

namespace EntComponents.ActorBehavior
{
    public class CubeActorBehavior(Entity host_entity) : EntComponent(host_entity)
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
            return 1;
        }

        protected override uint HandleCreate()
        {
            // Set the render's model and materials
            WorldRender renderer = (WorldRender)Host.GetComponent(typeof(WorldRender));
            renderer.SetModel(AssetLoader.ModelAssetGet("cube", AssetLoader.AssetSource.engine), AssetLoader.MaterialAssetGet("example", AssetLoader.AssetSource.engine));

            return 1;
        }

        protected override uint HandleUpdate()
        {
            return 1;
        }
    }
}