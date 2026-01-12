using Asset;
using Silk.NET.OpenGL;

namespace Engine
{
    /// <summary>
    /// Model asset, 
    /// </summary>
    public class AssetTexture : Asset
    {
        public AssetTexture(string asset_key, string file_path, TextureTarget tex_target) : base(asset_key, file_path)
        {
            // Compile source strings
            TextureData new_tex = new(file_path, tex_target);
            data = new_tex;
            asset_type = AssetType.textures;
        }

        public override void Unload()
        {
            (data as TextureData)?.Dispose();
            base.Unload();
        }
        
        public override bool CheckIntegrity(bool valid = true)
        {
            if(data == null) return false;
            TextureData check = (TextureData)data;
            return base.CheckIntegrity(check.IsValid());
        }
    }
}