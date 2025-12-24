
namespace Engine
{
    public static class AssetLoader
    {
        private static readonly List<int> asset_list = [];

        public static void LoadAsset(string asset_key, string file_path)
        {
            
            Console.WriteLine("Asset-> " + asset_key + " : " + file_path);
        }

        public static void UnloadAllAssets()
        {
            
        }
    }
}