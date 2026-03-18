using Engine;
using Silk.NET.OpenGL;

namespace Assets
{
    public class MaterialData : IDisposable
    {
        public MaterialData(List<TextureData> textures_data, Dictionary<string, object> mat_unitform_data, ShaderData shader, BlendingFactor blend_src = BlendingFactor.SrcAlpha, BlendingFactor blend_dest = BlendingFactor.OneMinusSrcAlpha)
        {
            render_shader = shader;
            uniforms = mat_unitform_data;
            textures = textures_data;
            BlendingSource = blend_src;
            BlendingDestination = blend_dest;
        }

        private readonly ShaderData render_shader;
        private Dictionary<string, object> uniforms;
        private List<TextureData> textures;

        public BlendingFactor BlendingSource { get; private set; }
        public BlendingFactor BlendingDestination { get; private set; }

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

        public Dictionary<string, object> Uniforms
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

        public void UseBlendMode()
        {
            Core.OpenGLContext.BlendFunc(BlendingSource, BlendingDestination);
        }

        public void Dispose()
        {

        }
    }
}