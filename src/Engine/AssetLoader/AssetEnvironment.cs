using Silk.NET.Assimp;
using Silk.NET.OpenGL;

namespace Engine
{
    /// <summary>
    /// Model asset, 
    /// </summary>
    public class AssetEnvironment : Asset
    {
        public AssetEnvironment(string asset_key, Environments.Environment enviro) : base(asset_key, "")
        {
            data = enviro;
            asset_type = AssetType.environment;
        }

        public override bool CheckIntegrity(bool valid = true)
        {
            if(data == null) return false;
            Environments.Environment check = (Environments.Environment)data;
            return base.CheckIntegrity(check.IsValid());
        }
    }
}