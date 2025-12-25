
namespace Engine
{
    public struct Asset
    {
        enum AssetType
        {
            Texture,
            Model,
            Sound,
            Shader
        }
    }

    public static class AssetLoader
    {
        private static Dictionary<string,Asset> asset_library = [];

        public static void LoadAsset(string asset_key, string file_path)
        {
            Asset created_asset = new Asset();
            asset_library.Add(asset_key, created_asset);

            Console.WriteLine("Asset-> " + asset_key + " : " + file_path);
        }
        
        public static Asset? GetAsset(string key)
        {
            asset_library.TryGetValue(key, out Asset found_asset);
            return found_asset;
        }

        public static void UnloadAllAssets()
        {
            asset_library = [];
        }
    }
}