namespace Engine
{
    /// <summary>
    /// Model asset, 
    /// </summary>
    public class AssetModel : Asset
    {
        public AssetModel(string asset_key, string file_path) : base(asset_key, file_path)
        {
            // Compile source strings
            Rendering.Model new_model = new(Core.OpenGLContext, file_path);
            data = new_model;
            asset_type = AssetType.model;
        }

        public override void Unload()
        {
            (data as Rendering.Model)?.Dispose();
            base.Unload();
        }
        
        public override bool CheckIntegrity(bool valid = true)
        {
            if(data == null) return false;
            Rendering.Model check = (Rendering.Model)data;
            return base.CheckIntegrity(check.IsValid());
        }
    }
}