using Assets;
using Engine;
using System.Numerics;

namespace EntComponents.Debuging
{
    public class DebugPlayer(Entity host_entity) : WorldRender(host_entity)
    {

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.render_priority, Core.Signals.hud_render];
        }

        public override uint HandleHudRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            CreateBaseUniforms2D(Matrix4x4.CreateScale(new Vector3(1f / Core.DisplayAspectRatio, 1f, 1f)), vertex_uniforms);

            Core.RenderText2D($"Test sprite", new Vector3(0f, 0f, 0f), 0.45f, 0.6f, vertex_uniforms);
/*
            Core.RenderSprite(Core.sprite2d_material, new Vector3(-0.5f, -0.5f, 0f), vertex_uniforms);
            Core.RenderSprite(Core.sprite2d_material, new Vector3(-0.5f, 0.5f, 0f), vertex_uniforms);
            Core.RenderSprite(Core.sprite2d_material, new Vector3(0.5f, 0.5f, 0f), vertex_uniforms);
            Core.RenderSprite(Core.sprite2d_material, new Vector3(0.5f, -0.5f, 0f), vertex_uniforms);

            
            Core.RenderSprite(Core.sprite2d_material, new Vector3(-1.5f, -1.5f, 0f), vertex_uniforms);
            
            Core.RenderSprite(Core.sprite2d_material, new Vector3(-2.5f, -2.5f, 0f), vertex_uniforms);
            
            Core.RenderSprite(Core.sprite2d_material, new Vector3(-3.5f, -3.5f, 0f), vertex_uniforms);
*/
            return 1;
        }
    }
}