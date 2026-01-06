using Silk.NET.Assimp;

namespace Rendering
{
    public class MaterialData : IDisposable
    {
        public MaterialData(List<TextureData> textures_data,  List<KeyValuePair<string,object>> mat_unitform_data, ShaderData shader)
        {
            render_shader = shader;
            uniforms = mat_unitform_data;
            textures = textures_data;
        }

        private readonly ShaderData render_shader;
        private List<KeyValuePair<string,object>> uniforms;
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

        public List<KeyValuePair<string,object>> Uniforms
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