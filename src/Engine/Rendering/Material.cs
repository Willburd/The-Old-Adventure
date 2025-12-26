using Silk.NET.Assimp;

namespace Rendering
{
    public struct MaterialUniformData(string uniform_name, int uni_value)
    {
        public string set_uniform = uniform_name;
        public int shader_uni_value = uni_value;
    }

    public class MaterialData : IDisposable
    {
        public MaterialData(List<MaterialUniformData> mat_tex_data, ShaderData shader)
        {
            render_shader = shader;
            uniforms = mat_tex_data;
        }

        private readonly ShaderData render_shader;
        private List<MaterialUniformData> uniforms;
        
        public ShaderData Shader
        {
            get
            {
                return render_shader;
            }
        }

        public bool IsValid()
        {
            return uniforms.Count > 0 && Shader.IsValid();
        }

        public List<MaterialUniformData> Uniforms
        {
            get
            {
                return uniforms;
            }
        }
        
        public void Dispose()
        {
            
        }
    }
}