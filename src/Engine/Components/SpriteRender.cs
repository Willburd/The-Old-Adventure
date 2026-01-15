using Engine;
using Assets;
using System.Diagnostics;
using System.Numerics;

namespace EntComponents
{
    public class SpriteRender : WorldRender
    {
        public SpriteRender(Entity host_entity) : base(host_entity)
        {
            model = Core.sprite2d_model;
            materials = [Core.sprite2d_material];
        }

        public override uint HandleRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            // position uniforms
            vertex_uniforms.Add(new("uTransform", Host.GetInterpolatedViewMatrix(tick_delta)));
            vertex_uniforms.Add(new("uView", Camera.GetCurrentInterpolatedViewMatrix(tick_delta)));
            vertex_uniforms.Add(new("uProjection", Camera.GetCurrentProjectionMatrix()));
            Core.RenderModel(model, materials, vertex_uniforms);
            return 1;
        }
    }
}