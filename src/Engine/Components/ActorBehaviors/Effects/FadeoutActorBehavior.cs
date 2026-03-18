using System.Numerics;
using Assets;
using Engine;

namespace EntComponents.ActorBehavior
{
    public class FadeoutActorBehavior : WorldRender
    {
        private const float fade_rate = 0.053f;
        public Vector3 FadeColor { get; set; }
        public FadeoutActorBehavior(Entity host_entity) : base(host_entity)
        {
            Priority = 50;
        }

        protected float fade_progress = 0f;

        public bool FadeComplete
        {
            get
            {
                return fade_progress > 1.1f; // Give a small buffer of fully black screen
            }
        }

        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.render_priority, Core.Signals.hud_render, Core.Signals.update];
        }

        protected override uint HandleUpdate()
        {
            fade_progress += fade_rate;
            return 1;
        }

        protected override uint HandleHudRender(double tick_delta, Dictionary<string, object> vertex_uniforms)
        {
            MaterialData fade_mat = AssetLoader.MaterialAssetGet("fade", AssetLoader.AssetSource.engine);
            fade_mat.Uniforms["uFade"] = Math.Clamp(fade_progress, 0f, 1f);
            Core.RenderSprite(fade_mat, HudCenter(), new Vector3(Core.DisplayAspectRatio, 1f, 1f), FadeColor, vertex_uniforms);
            return 1;
        }
    }
}