using Rendering;

namespace Engine
{
    /// <summary>
    /// Material asset, stores a collection of textures, uniform shader values and a shader for use.
    /// </summary>
    public class AssetMaterial : Asset
    {
        public AssetMaterial(string asset_key, MaterialData new_material) : base(asset_key, "")
        {
            // Get the shader program as our asset
            data = new_material;
            asset_type = AssetType.material;
        }

        public override void Unload()
        {
            (data as Rendering.MaterialData)?.Dispose();
            base.Unload();
        }
        
        public override bool CheckIntegrity(bool valid = true)
        {
            if(data == null) return false;
            Rendering.MaterialData check = (Rendering.MaterialData)data;
            return base.CheckIntegrity(check.IsValid());
        }
    }
}