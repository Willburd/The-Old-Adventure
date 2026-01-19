using Engine;
using System.Numerics;
using Assets;

namespace EntComponents.ActorBehavior
{
    public class FireActorBehavior(Entity host_entity) : EntComponent(host_entity)
    {
        public int DecayTimer { get; set; } = 0;
        public Vector4 FireColor { get; set; } = new Vector4(1f, 1f, 1f, 1f);

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
            // Shaders
            ShaderData shader_fire = AssetLoader.ShaderAssetLoad("fire_scroll", "unshaded.vert", "fire.frag", AssetLoader.AssetSource.engine);
            
            // Textures
            TextureData fire_tex = AssetLoader.TextureAssetLoad("Effects/fire_scroll.png", Silk.NET.OpenGL.TextureTarget.Texture2D, AssetLoader.AssetSource.engine);

            // Materials
            AssetLoader.MaterialAssetLoad("fire_scroll", new([fire_tex], [new("uTexture0", 0), new("uFireColor", new Vector4(1.0f, 0.55f, 0.0f, 1.0f))], shader_fire), AssetLoader.AssetSource.engine);

            // Models
            BillboardRender render = (BillboardRender)Host.GetComponent(typeof(BillboardRender));
            render.SetMaterial(AssetLoader.MaterialAssetGet("fire_scroll", AssetLoader.AssetSource.engine));
            render.Priority += 5;
            Host.Scale = new Vector3(0.5f, 1.25f, 1.25f);

            Light light = (Light)Host.GetComponent(typeof(Light));
            light.SetData(FireColor, 12f);

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
                float light_scale_rate = 0.92f;
                Host.Scale *= light_scale_rate;
                Light light = (Light)Host.GetComponent(typeof(Light));
                light.Radius *= light_scale_rate;
                Host.Position += Tools.Down * 0.025f * Host.Scale.Length(); // Needs to offset scaling
                return 1;
            }
            Host.Scale *= 0f;
            DecayTimer = 0;
            Host.Destroy();
            return 1;
        }
    }
}