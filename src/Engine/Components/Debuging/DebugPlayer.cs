using Assets;
using Engine;
using System.Numerics;

namespace EntComponents.Debuging
{
    public class DebugPlayer(Entity host_entity) : WorldRender(host_entity)
    {
        float test = 0f;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.render_priority, Core.Signals.update, Core.Signals.hud_render];
        }

        protected override uint HandleUpdate()
        {
            test += 0.04f;

            return 1;
        }

        protected override uint HandleHudRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {

            Core.RenderSprite(Core.sprite2d_material, HudBottomLeft(), Vector3.One * 0.2f, vertex_uniforms);
            Core.RenderSprite(Core.sprite2d_material, HudBottomRight(), Vector3.One * 0.1f, vertex_uniforms);
            Core.RenderSprite(Core.sprite2d_material, HudTopLeft(), Vector3.One * 0.1f, vertex_uniforms);
            Core.RenderSprite(Core.sprite2d_material, HudTopRight(), Vector3.One * 0.2f, vertex_uniforms);

            
            Core.RenderSprite(Core.sprite2d_material, HudLerp(0.5f + MathF.Sin(test) * 0.5f, 0.5f + MathF.Cos(test) * 0.5f), Vector3.One * 0.3f, vertex_uniforms);

            Core.RenderText2D($"This is a test\nof the hud\nrender system.", HudLerp(0.5f + MathF.Cos(test) * 0.5f, 0.5f + MathF.Sin(test) * 0.5f), Vector3.One * 0.1f, vertex_uniforms);

            return 1;
        }
    }
}