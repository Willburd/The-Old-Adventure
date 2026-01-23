using Engine;
using Assets;
using System.Numerics;

namespace EntComponents
{
    public class SpriteRender : WorldRender
    {
        public Vector2 CutoutPosition { get; set; } = Vector2.Zero;
        public Vector2 CutoutSize { get; set; } = Vector2.One;
        public Vector3 DrawOffset { get; set; } = Vector3.Zero;

        public SpriteRender(Entity host_entity) : base(host_entity)
        {
            model = Core.sprite2d_model;
            materials = [Core.sprite2d_material];
        }

        public override uint HandleRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            CreateBaseUniforms3D(Host.GetInterpolatedViewMatrix(tick_delta), tick_delta, vertex_uniforms);
            vertex_uniforms.Add(new("uSpritePos", CutoutPosition));
            vertex_uniforms.Add(new("uSpriteSize", CutoutSize));
            vertex_uniforms.Add(new("uDrawOffset", Matrix4x4.CreateTranslation(DrawOffset)));
            Core.RenderModel(model, materials, vertex_uniforms);
            return 1;
        }
    }
}