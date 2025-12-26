using System.Diagnostics;

namespace Engine
{
    public static partial class AssetLoader
    {
        private static Dictionary<string,Asset> asset_library = [];

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Asset management
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public static void RemoveAsset(string asset_key)
        {
            if(!asset_library.TryGetValue(asset_key, out Asset? value)) return;
            value.Unload();
            asset_library.Remove(asset_key);
        }

        public static void UnloadAllAssets(bool even_persistent)
        {
            List<string> removed_keys = [];
            foreach((string key, Asset asset) in asset_library)
            {
                if(!asset.Persistent || even_persistent)
                {
                    asset.Unload();
                    removed_keys.Add(key);
                }
            }
            foreach(string key in removed_keys)
            {
                asset_library.Remove(key);
            }
        }

        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Asset loading
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        /// <summary>
        /// Add asset to the asset library, 
        /// </summary>
        private static void AddAsset(string asset_key, Asset new_asset)
        {
            if(asset_library.ContainsKey(asset_key)) 
            {
                if(asset_library[asset_key].Persistent) return;
                RemoveAsset(asset_key); // Clear it and recall it!
            }
            if(!new_asset.CheckIntegrity())
            {
                Console.WriteLine("ASSET LOAD FAILURE - " + asset_key + " : " + new_asset.FilePath);
            }
            asset_library.Add(asset_key, new_asset);
        }
        
        /// <summary>
        /// Loads a shader asset from disk. Internal use, use the type specific ones instead.
        /// </summary>
        private static void LoadAsset(string asset_key, string file_path)
        {
            // TODO open file for asset create file_path
            AddAsset(asset_key, new Asset(asset_key, file_path));
        }

        /// <summary>
        /// Checks that an asset is loaded.
        /// </summary>
        public static bool AssetExists(string asset_key)
        {
            return asset_library.ContainsKey(asset_key);
        }
        
        /// <summary>
        /// Loads a shader asset from disk into the asset library
        /// </summary>
        public static void ShaderAssetLoad(string asset_key, string file_path_without_type)
        {
            AddAsset(asset_key, new AssetShader(asset_key, file_path_without_type));
        }

        /// <summary>
        /// Loads a 3d Model asset from disk into the asset library
        /// </summary>
        public static void ModelAssetLoad(string asset_key, string file_path)
        {
            AddAsset(asset_key, new AssetModel(asset_key, file_path));
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Asset retrieval
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        /// <summary>
        /// Get an asset from asset_library. The asset must exist.
        /// </summary>
        public static Asset GetAsset(string asset_key)
        {
            asset_library.TryGetValue(asset_key, out Asset? found_asset);
            Debug.Assert(found_asset != null);
            return found_asset;
        }
        
        /// <summary>
        /// Get a shader program from the asset key. The asset must exist.
        /// </summary>
        public static Rendering.Shader ShaderAssetGet(string asset_key)
        {
            Asset ast = GetAsset(asset_key);
            Debug.Assert(ast.CheckType(Asset.AssetType.shader));
            return (Rendering.Shader)ast.Data;
        }
        
        /// <summary>
        /// Get a Assimp scene of a model. The asset must exist.
        /// </summary>
        public static Rendering.Model ModelAssetGet(string asset_key)
        {
            Asset ast = GetAsset(asset_key);
            Debug.Assert(ast.CheckType(Asset.AssetType.model));
            return (Rendering.Model)ast.Data;
        }
    }
}