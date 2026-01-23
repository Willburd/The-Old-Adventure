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
            // position uniforms
            vertex_uniforms.Add(new("uTransform", Matrix4x4.CreateScale(new Vector3(0.11f, 0.1f, 1f))));
            vertex_uniforms.Add(new("uProjection", Matrix4x4.CreateOrthographic(1, 1, 0.0001f, 10000f)));
            vertex_uniforms.Add(new("uView", Matrix4x4.CreateFromQuaternion(Quaternion.Identity) * Matrix4x4.CreateTranslation(Tools.Forward)));

            Core.RenderText2D($"Test sprite", new Vector3(0f, 0f, 0f), 0.45f, 0.6f, vertex_uniforms);

            Core.RenderSprite(Core.sprite2d_material, new Vector3(-0.5f, -0.5f, 0f), vertex_uniforms);
            Core.RenderSprite(Core.sprite2d_material, new Vector3(-0.5f, 0.5f, 0f), vertex_uniforms);
            Core.RenderSprite(Core.sprite2d_material, new Vector3(0.5f, 0.5f, 0f), vertex_uniforms);
            Core.RenderSprite(Core.sprite2d_material, new Vector3(0.5f, -0.5f, 0f), vertex_uniforms);

            
            Core.RenderSprite(Core.sprite2d_material, new Vector3(-1.5f, -1.5f, 0f), vertex_uniforms);
            
            Core.RenderSprite(Core.sprite2d_material, new Vector3(-2.5f, -2.5f, 0f), vertex_uniforms);
            
            Core.RenderSprite(Core.sprite2d_material, new Vector3(-3.5f, -3.5f, 0f), vertex_uniforms);
            return 1;
        }
    }
}