using Assets;

namespace Engine
{
    /// <summary>
    /// Environment asset, stores environment property data.
    /// </summary>
    public class AssetEnvironment : LoadedAsset
    {
        public AssetEnvironment(string asset_key, EnvironmentData enviro) : base(asset_key, "")
        {
            data = enviro;
            asset_type = AssetType.environment;
        }

        public override bool CheckIntegrity(bool valid = true)
        {
            if (data == null) return false;
            EnvironmentData check = (EnvironmentData)data;
            return base.CheckIntegrity(check.IsValid());
        }
    }
}