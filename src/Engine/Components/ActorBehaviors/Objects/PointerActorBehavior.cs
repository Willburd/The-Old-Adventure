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
            return 1;
        }

        protected override uint HandleCreate()
        {
            // Set the render's model and materials
            WorldRender renderer = (WorldRender)Host.GetComponent(typeof(WorldRender));
            renderer.SetModel(AssetLoader.ModelAssetGet("pointer", AssetLoader.AssetSource.engine), AssetLoader.MaterialAssetGet("example", AssetLoader.AssetSource.engine));

            return 1;
        }

        protected override uint HandleUpdate()
        {
            Actor find_player = Actor.GetActor(PlayerActorBehavior.player_actor_id);
            if (find_player != null)
            {
                Host.Rotation = Tools.LookAt(Host.Position, find_player.Position);
            }
            return 1;
        }
    }
}