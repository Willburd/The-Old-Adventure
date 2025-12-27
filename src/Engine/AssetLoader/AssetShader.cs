namespace Engine
{
    /// <summary>
    /// Shader asset, stores a compiled shader's index in the GL context. Either loaded from a file or directly compiled.
    /// </summary>
    public class AssetShader : Asset
    {
        public AssetShader(string asset_key, string vertex_shader, string frag_shader) : base(asset_key, frag_shader)
        {
            // Compile source strings
            Rendering.ShaderData shader_data = new(vertex_shader, frag_shader);
            // Get the shader program as our asset
            data = shader_data;
            asset_type = AssetType.shader;
        }

        public override void Unload()
        {
            (data as Rendering.ShaderData)?.Dispose();
            base.Unload();
        }
        
        public override bool CheckIntegrity(bool valid = true)
        {
            if(data == null) return false;
            Rendering.ShaderData check = (Rendering.ShaderData)data;
            return base.CheckIntegrity(check.IsValid());
        }
    }
}