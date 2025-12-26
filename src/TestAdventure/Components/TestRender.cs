using Engine;
using Silk.NET.OpenGL;

namespace EntComponents
{
    public class TestRender : Renders
    {
        public unsafe TestRender(Entity host_entity) : base(host_entity)
        {
            // Setup vertex data
            Vertices =
            [
                //X    Y      Z
                0.5f,  0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                -0.5f, -0.5f, 0.0f,
                -0.5f,  0.5f, 0.5f
            ];

            //Index data, uploaded to the EBO.
            Indices =
            [
                0, 1, 3,
                1, 2, 3
            ];

            // Setup the base bindings
            InitBindings();

            // Tell opengl how to give the data to the shaders.
            Shader = AssetLoader.ShaderAssetGet("shader_test");
            Gl.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, false, 3 * sizeof(float), null);
            Gl.EnableVertexAttribArray(0);
        }

        public unsafe override void HandleRender(double delta_time)
        {
            // Draw test quad
            Gl.BindVertexArray(Vao);
            Shader.Use();

            //Draw the geometry.
            Gl.DrawElements(PrimitiveType.Triangles, (uint) Indices.Length, DrawElementsType.UnsignedInt, null);
        }
    }
}