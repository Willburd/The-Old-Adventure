using Silk.NET.Assimp;

namespace Rendering
{
    public struct MaterialTexData(TextureData tex, TextureType texture_type)
    {
        public TextureData texture_data = tex;
        public TextureType texture_type = texture_type;
    }

    public class MaterialData : IDisposable
    {
        public MaterialData(List<MaterialTexData> mat_tex_data, ShaderData shader)
        {
            render_shader = shader;
            textures = mat_tex_data;
        }

        private readonly ShaderData render_shader;
        private List<MaterialTexData> textures;
        
        public ShaderData Shader
        {
            get
            {
                return render_shader;
            }
        }

        public bool IsValid()
        {
            return textures.Count > 0 && Shader.IsValid();
        }
        
        public void Dispose()
        {
            
        }
    }
}