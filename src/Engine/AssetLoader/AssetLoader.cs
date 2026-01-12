using System.Diagnostics;
using Asset;

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

        /// <summary>
        /// Unloads all non-persistent assets. Called by shutdown with a flag to delete even the persistent assets.
        /// </summary>
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

        /// <summary>
        /// Used for flagging the base assets as persistently loaded. Shouldn't really be used anywhere else.
        /// </summary>
        public static void PersistAllAssets()
        {
            foreach((string key, Asset asset) in asset_library)
            {
                asset.SetPersistent();
            }
        }

        public enum AssetSource
        {
            engine,
            adventure
        }

        /// <summary>
        /// Creates asset keys based on the type of asset and if it is an engine or adventure resource.
        /// </summary>
        public static string AssetKey(Asset.AssetType type, string asset_key, AssetSource asset_source = AssetSource.adventure)
        {
            string src_str = "Engine"; // We default to the engine if no arguments are provided, as this will usually be called by the adventure for non-engine assets anyway.
            if(Core.AdventureID != null && asset_source == AssetSource.adventure) src_str = Core.AdventureID;
            return src_str + "::" + type + "::" + asset_key;
        }

        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Asset loading
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        /// <summary>
        /// Add asset to the asset library, weirdly named to avoid accidental use. Use the proper load asset functions.
        /// </summary>
        private static Object InvokeAsset(string asset_key, Asset new_asset)
        {
            if(!new_asset.CheckIntegrity()) Console.WriteLine("ASSET LOAD FAILURE - " + asset_key + " : " + new_asset.FilePath);
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
        public static ShaderData ShaderAssetLoad(string asset_key, string vertext_path, string frag_path, AssetSource source = AssetSource.adventure)
        {
            string get_key = AssetLoader.AssetKey(Asset.AssetType.shader, asset_key, source);
            if(asset_library.ContainsKey( get_key)) return (ShaderData)asset_library[get_key].Data;
            // Check if exists
            if(!File.Exists(vertext_path) || !File.Exists(frag_path)) 
            {
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=");
                if(!File.Exists(vertext_path)) Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X BAD SHADER, FILE DOES NOT EXIST : " + vertext_path);
                if(!File.Exists(frag_path)) Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X BAD SHADER, FILE DOES NOT EXIST : " + frag_path);
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=");
                return (ShaderData)LocateAsset(AssetLoader.AssetKey(Asset.AssetType.shader, "standard", AssetSource.engine)).Data;
            }
            return (ShaderData)InvokeAsset( get_key, new AssetShader(get_key, vertext_path, frag_path));
        }

        /// <summary>
        /// Loads a 3d Model asset from disk into the asset library
        /// </summary>
        public static ModelData ModelAssetLoad(string asset_key, string file_path, AssetSource source = AssetSource.adventure)
        {
            string get_key = AssetLoader.AssetKey(Asset.AssetType.model, asset_key, source);
            if(asset_library.ContainsKey(get_key)) return (ModelData)asset_library[get_key].Data;
            // Check if exists
            if(!File.Exists(file_path)) 
            {
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=");
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X BAD MODEL, FILE DOES NOT EXIST : " + file_path);
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=");
                return (ModelData)LocateAsset(AssetLoader.AssetKey(Asset.AssetType.model, "no_model", AssetSource.engine)).Data;
            }
            return (ModelData)InvokeAsset( get_key, new AssetModel(get_key, file_path));
        }
        
        /// <summary>
        /// Loads a texture asset from disk into the asset library
        /// </summary>
        public static TextureData TextureAssetLoad(string asset_key, string file_path, Silk.NET.OpenGL.TextureTarget tex_target, AssetSource source = AssetSource.adventure)
        {
            string get_key = AssetLoader.AssetKey(Asset.AssetType.textures, asset_key, source);
            if(asset_library.ContainsKey( get_key)) return (TextureData)asset_library[get_key].Data;
            // Check if exists
            if(!File.Exists(file_path)) 
            {
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=");
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X BAD TEXTURE, FILE DOES NOT EXIST : " + file_path);
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=");
                return (TextureData)LocateAsset(AssetLoader.AssetKey(Asset.AssetType.textures, "no_tex", AssetSource.engine)).Data;
            }
            return (TextureData)InvokeAsset( get_key, new AssetTexture(get_key, file_path, tex_target));
        }

        /// <summary>
        /// Creates a material asset and adds it to the asset library for reuse
        /// </summary>
        public static MaterialData MaterialAssetLoad(string asset_key, MaterialData new_material, AssetSource source = AssetSource.adventure)
        {
            string get_key = AssetLoader.AssetKey(Asset.AssetType.material, asset_key, source);
            if(asset_library.ContainsKey( get_key)) return (MaterialData)asset_library[get_key].Data;
            return (MaterialData)InvokeAsset( get_key, new AssetMaterial(get_key, new_material));
        }
        
        /// <summary>
        /// Creates an environment asset and adds it to the asset library for reuse
        /// </summary>
        public static Environments.Environment EnvironmentAssetLoad(Environments.Environment new_environment, AssetSource source = AssetSource.adventure)
        {
            string get_key = AssetLoader.AssetKey(Asset.AssetType.environment, new_environment.AssetKey, source);
            if(asset_library.ContainsKey( get_key)) return (Environments.Environment)asset_library[get_key].Data;
            return (Environments.Environment)InvokeAsset( get_key, new AssetEnvironment(get_key, new_environment));
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
        /// Get an asset from asset_library. The asset must exist, weirdly named to avoid accidental use. Use the proper get asset functions.
        /// </summary>
        public static Asset LocateAsset(string asset_key)
        {
            asset_library.TryGetValue(asset_key, out Asset? found_asset);
            return found_asset;
        }
        
        /// <summary>
        /// Gets a shader from the asset library
        /// </summary>
        /// 
        public static ShaderData ShaderAssetGet(string asset_key, AssetSource source = AssetSource.adventure)
        {
            Asset ast = LocateAsset(AssetLoader.AssetKey(Asset.AssetType.shader, asset_key, source));
            Debug.Assert(ast.CheckType(Asset.AssetType.shader));
            return (ShaderData)ast.Data;
        }
        
        /// <summary>
        /// Gets a Model from the asset library
        /// </summary>
        public static ModelData ModelAssetGet(string asset_key, AssetSource source = AssetSource.adventure)
        {
            Asset ast = LocateAsset(AssetLoader.AssetKey(Asset.AssetType.model, asset_key, source));
            ast ??= LocateAsset(AssetLoader.AssetKey(Asset.AssetType.model, "no_model", AssetSource.engine));
            Debug.Assert(ast.CheckType(Asset.AssetType.model));
            return (ModelData)ast.Data;
        }

        /// <summary>
        /// Gets a texture from the asset library
        /// </summary>
        public static TextureData TextureAssetGet(string asset_key, AssetSource source = AssetSource.adventure)
        {
            Asset ast = LocateAsset(AssetLoader.AssetKey(Asset.AssetType.textures, asset_key, source));
            ast ??= LocateAsset(AssetLoader.AssetKey(Asset.AssetType.textures, "no_tex", AssetSource.engine));
            Debug.Assert(ast.CheckType(Asset.AssetType.textures));
            return (TextureData)ast.Data;
        }

        /// <summary>
        /// Gets a material from the asset library
        /// </summary>
        public static MaterialData MaterialAssetGet(string asset_key, AssetSource source = AssetSource.adventure)
        {
            Asset ast = LocateAsset(AssetLoader.AssetKey(Asset.AssetType.material, asset_key, source));
            ast ??= LocateAsset(AssetLoader.AssetKey(Asset.AssetType.material, "no_mat", AssetSource.engine));
            Debug.Assert(ast.CheckType(Asset.AssetType.material));
            return (MaterialData)ast.Data;
        }

        /// <summary>
        /// Gets an environment from the asset library
        /// </summary>
        public static Environments.Environment EnvironmentAssetGet(string asset_key, AssetSource source = AssetSource.adventure)
        {
            Asset ast = LocateAsset(AssetLoader.AssetKey(Asset.AssetType.environment, asset_key, source));
            ast ??= LocateAsset(AssetLoader.AssetKey(Asset.AssetType.environment, "standard_day", AssetSource.engine));
            Debug.Assert(ast.CheckType(Asset.AssetType.environment));
            return (Environments.Environment)ast.Data;
        }
    }
}