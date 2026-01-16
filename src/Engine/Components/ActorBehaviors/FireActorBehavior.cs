using Engine;
using System.Numerics;

namespace EntComponents.ActorBehavior
{
    public class FireActorBehavior(Entity host_entity) : EntComponent(host_entity)
    {
        public int DecayTimer { get; set; } = 0;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public override List<Core.Signals> PrepareSignals()
        {
            // We still register update, just incase something external gives us a decay timer. Like wind blowing out torches in a cutscene.
            return [Core.Signals.load_assets, Core.Signals.create, Core.Signals.update];
        }

        protected override uint HandleAssetLoad()
        {
            BillboardRender render = (BillboardRender)Host.GetComponent(typeof(BillboardRender));
            render.SetMaterial(AssetLoader.MaterialAssetGet("fire"));

            Light light = (Light)Host.GetComponent(typeof(Light));
            light.SetData(new Vector4(1f, 1f, 1f, 1f), 7f);

            return 1;
        }

        protected override uint HandleUpdate()
        {
            // If we don't have a decay timer, than we don't decay!
            if (DecayTimer == 0) return 0; 
            if (DecayTimer > 1)
            {
                DecayTimer--;
                return 1;
            }
            // Shrink and then destroy
            if (Host.Scale.Length() > 0.01f)
            {
                Host.Scale *= 0.92f;
                return 1;
            }
            Host.Scale *= 0f;
            DecayTimer = 0;
            Host.Destroy();
            return 1;
        }
    }
}