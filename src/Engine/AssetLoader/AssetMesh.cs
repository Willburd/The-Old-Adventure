using Silk.NET.Assimp;

namespace Engine
{
    public class AssetMesh : Asset
    {
        public AssetMesh(string asset_key, string file_path) : base(asset_key, file_path)
        {

            asset_type = AssetType.mesh;
        }

        public override void Unload()
        {
            
            base.Unload();
        }
        
        public override bool CheckIntegrity(bool valid = true)
        {
            return base.CheckIntegrity(true);
        }
    }
}