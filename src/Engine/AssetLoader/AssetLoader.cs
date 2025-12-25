using System.Diagnostics;
using Silk.NET.OpenGL;

namespace Engine
{
    public class Asset(Object new_asset)
    {
        protected readonly Object data = new_asset;

        public Object GetAsset 
        {
            get
            {
                return data;
            }
        }
    }

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
            asset_library.Add(asset_key, new_asset);
            Console.WriteLine("Asset-> " + asset_key);
        }

        public static Asset GetAsset(string asset_key)
        {
            asset_library.TryGetValue(asset_key, out Asset? found_asset);
            Debug.Assert(found_asset != null);
            return found_asset;
        }

        public static void UnloadAllAssets()
        {
            asset_library = [];
        }
    }
}