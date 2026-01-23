using Assets;
using Engine;

namespace EntComponents.ActorBehavior
{
    public class FadeoutActorBehavior : WorldRender
    {
        public FadeoutActorBehavior(Entity host_entity) : base(host_entity)
        {
            Priority = 30;
        }

        private float fade_progress = 0f;

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
            fade_progress += 0.01f;
            return 1;
        }

        protected override uint HandleHudRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            

            return 1;
        }
    }
}