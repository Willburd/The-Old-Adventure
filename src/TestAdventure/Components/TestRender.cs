using Engine;
using Silk.NET.OpenGL;

namespace EntComponents
{
    public class TestRender(Entity host_entity) : Renders(host_entity)
    {
        private Rendering.Model model;
        protected Rendering.Shader shader;

        public override uint HandleAssetLoad()
        {
            AssetLoader.ShaderAssetLoad("shader_test", Tools.AssetDirectory + "/Shaders/test");
            AssetLoader.ModelAssetLoad("model_test", Tools.AssetDirectory + "/Models/test.obj");

            shader = AssetLoader.ShaderAssetGet("shader_test");
            model = AssetLoader.ModelAssetGet("model_test");

            return 1;
        }

        public override uint HandleCreate()
        {
            return 1;
        }

        public override uint HandleRender(double delta_time)
        {  
            foreach (var mesh in model.Meshes)
            {
                mesh.Bind();
                shader.Use();
                Core.OpenGLContext.DrawArrays(PrimitiveType.Triangles, 0, (uint)mesh.Vertices.Length);
            }
            return 1;
        }
    }
}