using System.Diagnostics;
using Rendering;

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
        /// Add asset to the asset library, weirdly named to avoid accidental use. Use the proper load asset functions.
        /// </summary>
        private static Object InvokeAsset(string asset_key, Asset new_asset)
        {
            if(asset_library.ContainsKey(asset_key)) 
            {
                if(asset_library[asset_key].Persistent) return asset_library[asset_key].Data;
                RemoveAsset(asset_key); // Clear it and recall it!
            }
            if(!new_asset.CheckIntegrity())
            {
                Console.WriteLine("ASSET LOAD FAILURE - " + asset_key + " : " + new_asset.FilePath);
            }
            asset_library.Add(asset_key, new_asset);
            return new_asset.Data;
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
        public static Rendering.ShaderData ShaderAssetLoad(string asset_key, string file_path_without_type)
        {
            return (ShaderData)InvokeAsset(asset_key, new AssetShader(asset_key, file_path_without_type));
        }

        /// <summary>
        /// Loads a 3d Model asset from disk into the asset library
        /// </summary>
        public static ModelData ModelAssetLoad(string asset_key, string file_path)
        {
            return (ModelData)InvokeAsset(asset_key, new AssetModel(asset_key, file_path));
        }
        
        /// <summary>
        /// Loads a 3d Model asset from disk into the asset library
        /// </summary>
        public static TextureData TextureAssetLoad(string asset_key, string file_path)
        {
            return (TextureData)InvokeAsset(asset_key, new AssetTexture(asset_key, file_path));
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Asset retrieval
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        public static string WorkingDirectory
        {
            get
            {
                // TODO - get this to not be godawful and actually use the project folder
                return AppDomain.CurrentDomain.BaseDirectory + "../../";
            }
        }

        public static string AssetDirectoryAdventure
        {
            get
            {
                return WorkingDirectory + "Assets/" + Core.AdventureID;
            }
        }

        public static string AssetDirectoryEngine
        {
            get
            {
                return WorkingDirectory + "Assets/Engine";
            }
        }

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
        /// Gets a shader from the asset library
        /// </summary>
        public static Rendering.ShaderData ShaderAssetGet(string asset_key)
        {
            Asset ast = GetAsset(asset_key);
            Debug.Assert(ast.CheckType(Asset.AssetType.shader));
            return (Rendering.ShaderData)ast.Data;
        }
        
        /// <summary>
        /// Gets a Model from the asset library
        /// </summary>
        public static Rendering.ModelData ModelAssetGet(string asset_key)
        {
            Asset ast = GetAsset(asset_key);
            Debug.Assert(ast.CheckType(Asset.AssetType.model));
            return (Rendering.ModelData)ast.Data;
        }

        /// <summary>
        /// Gets a texture from the asset library
        /// </summary>
        public static Rendering.TextureData TextureAssetGet(string asset_key)
        {
            Asset ast = GetAsset(asset_key);
            Debug.Assert(ast.CheckType(Asset.AssetType.textures));
            return (Rendering.TextureData)ast.Data;
        }
    }
}