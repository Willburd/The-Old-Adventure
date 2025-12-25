using System.Diagnostics;

namespace Engine
{
    public static partial class AssetLoader
    {
        private static Dictionary<string,Asset> asset_library = [];

        public static void Init()
        {
            Gl = Core.OpenGLContext;
        }

        public static void LoadAsset(string asset_key, string file_path)
        {
            // TODO open file for asset create file_path
            AddAsset(asset_key, new Asset(100));
        }
        
        private static void AddAsset(string asset_key, Asset new_asset)
        {
            if(asset_library.ContainsKey(asset_key)) 
            {
                if(asset_library[asset_key].Persistent) return;
                RemoveAsset(asset_key); // Clear it and recall it!
            }
            asset_library.Add(asset_key, new_asset);
            Console.WriteLine("Asset-> " + asset_key);
        }

        public static Asset GetAsset(string asset_key)
        {
            asset_library.TryGetValue(asset_key, out Asset? found_asset);
            Debug.Assert(found_asset != null);
            return found_asset;
        }

        public static void RemoveAsset(string asset_key)
        {
            if(!asset_library.ContainsKey(asset_key)) return;
            asset_library[asset_key].Cleanup();
            asset_library.Remove(asset_key);
        }

        public static void UnloadAllAssets(bool even_persistent)
        {
            List<string> removed_keys = [];
            foreach((string key, Asset asset) in asset_library)
            {
                if(!asset.Persistent || even_persistent)
                {
                    asset.Cleanup();
                    removed_keys.Add(key);
                }
            }
            foreach(string key in removed_keys)
            {
                asset_library.Remove(key);
            }
        }
    }
}