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
        public MaterialData(List<TextureData> textures_data,  List<MaterialUniformData> mat_unitform_data, ShaderData shader)
        {
            render_shader = shader;
            uniforms = mat_unitform_data;
            textures = textures_data;
        }

        private readonly ShaderData render_shader;
        private List<MaterialUniformData> uniforms;
        private List<TextureData> textures;
        
        public ShaderData Shader
        {
            get
            {
                return render_shader;
            }
        }

        public bool IsValid()
        {
            return Shader.IsValid();
        }

        public List<MaterialUniformData> Uniforms
        {
            get
            {
                return uniforms;
            }
        }

        public List<TextureData> Textures
        {
            get
            {
                return textures;
            }
        }
        
        public void Dispose()
        {
            
        }
    }
}