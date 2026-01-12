using Assets;

namespace Engine
{
    /// <summary>
    /// Model asset, 
    /// </summary>
    public class AssetModel : LoadedAsset
    {
        public AssetModel(string asset_key, string file_path) : base(asset_key, file_path)
        {
            // Compile source strings
            ModelData new_model = new(file_path);
            data = new_model;
            asset_type = AssetType.model;
        }

        public override void Unload()
        {
            (data as ModelData)?.Dispose();
            base.Unload();
        }
        
        public override bool CheckIntegrity(bool valid = true)
        {
            if(data == null) return false;
            ModelData check = (ModelData)data;
            return base.CheckIntegrity(check.IsValid());
        }
    }
}