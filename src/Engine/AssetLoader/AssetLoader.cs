using System.Diagnostics;
using Assets;

namespace Engine
{
    public static partial class AssetLoader
    {
        private static Dictionary<string, LoadedAsset> asset_library = [];

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Assets management
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public static void RemoveAsset(string asset_key)
        {
            if (!asset_library.TryGetValue(asset_key, out LoadedAsset value)) return;
            value.Unload();
            asset_library.Remove(asset_key);
        }

        /// <summary>
        /// Unloads all non-persistent assets. Called by shutdown with a flag to delete even the persistent assets.
        /// </summary>
        public static void UnloadAllAssets(bool even_persistent)
        {
            List<string> removed_keys = [];
            foreach ((string key, LoadedAsset asset) in asset_library)
            {
                if (!asset.Persistent || even_persistent)
                {
                    asset.Unload();
                    removed_keys.Add(key);
                }
            }
            foreach (string key in removed_keys)
            {
                asset_library.Remove(key);
            }
        }

        /// <summary>
        /// Used for flagging the base assets as persistently loaded. Shouldn't really be used anywhere else.
        /// </summary>
        public static void PersistAllAssets()
        {
            foreach ((string key, LoadedAsset asset) in asset_library)
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
        public static string AssetKey(LoadedAsset.AssetType type, string asset_key, AssetSource asset_source = AssetSource.adventure)
        {
            string src_str = "Engine"; // We default to the engine if no arguments are provided, as this will usually be called by the adventure for non-engine assets anyway.
            if (Core.AdventureID != null && asset_source == AssetSource.adventure) src_str = Core.AdventureID;
            return src_str + "::" + type + "::" + asset_key;
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Assets loading
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Add asset to the asset library, weirdly named to avoid accidental use. Use the proper load asset functions.
        /// </summary>
        private static Object InvokeAsset(string asset_key, LoadedAsset new_asset)
        {
            if (!new_asset.CheckIntegrity()) Console.WriteLine("ASSET LOAD FAILURE - " + asset_key + " : " + new_asset.FilePath);
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
            vertext_path = (source == AssetSource.engine) ? AssetDirectoryEngine + "/Shaders/" + vertext_path : AssetDirectoryAdventure + "/Shaders/" + vertext_path;
            frag_path = (source == AssetSource.engine) ? AssetDirectoryEngine + "/Shaders/" + frag_path : AssetDirectoryAdventure + "/Shaders/" + frag_path;

            string get_key = AssetLoader.AssetKey(LoadedAsset.AssetType.shader, asset_key, source);
            if (asset_library.TryGetValue(get_key, out LoadedAsset? value)) return (ShaderData)value.Data;
            // Check if exists
            if (!File.Exists(vertext_path) || !File.Exists(frag_path))
            {
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=");
                if (!File.Exists(vertext_path)) Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X BAD SHADER, FILE DOES NOT EXIST : " + vertext_path);
                if (!File.Exists(frag_path)) Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X BAD SHADER, FILE DOES NOT EXIST : " + frag_path);
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=");
                return (ShaderData)LocateAsset(AssetLoader.AssetKey(LoadedAsset.AssetType.shader, "standard", AssetSource.engine)).Data;
            }
            return (ShaderData)InvokeAsset(get_key, new AssetShader(get_key, vertext_path, frag_path));
        }

        /// <summary>
        /// Loads a 3d Model asset from disk into the asset library, asset key is the same as the file name
        /// </summary>
        public static ModelData ModelAssetLoad(string file_path, AssetSource source = AssetSource.adventure)
        {
            return ModelAssetLoad(Path.GetFileNameWithoutExtension(file_path), file_path, source);
        }

        /// <summary>
        /// Loads a 3d Model asset from disk into the asset library, with a specific asset key
        /// </summary>
        public static ModelData ModelAssetLoad(string asset_key, string file_path, AssetSource source = AssetSource.adventure)
        {
            file_path = source == AssetSource.engine ? AssetDirectoryEngine + "/Models/" + file_path : AssetDirectoryAdventure + "/Models/" + file_path;
            string get_key = AssetLoader.AssetKey(LoadedAsset.AssetType.model, asset_key, source);
            if (asset_library.TryGetValue(get_key, out LoadedAsset? value)) return (ModelData)value.Data;
            // Check if exists
            if (!File.Exists(file_path))
            {
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=");
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X BAD MODEL, FILE DOES NOT EXIST : " + file_path);
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=");
                return (ModelData)LocateAsset(AssetLoader.AssetKey(LoadedAsset.AssetType.model, "no_model", AssetSource.engine)).Data;
            }
            return (ModelData)InvokeAsset(get_key, new AssetModel(get_key, file_path));
        }

        /// <summary>
        /// Loads a texture asset from disk into the asset library, asset key is the same as the file name
        /// </summary>
        public static TextureData TextureAssetLoad(string file_path, Silk.NET.OpenGL.TextureTarget tex_target, AssetSource source = AssetSource.adventure)
        {
            return TextureAssetLoad(Path.GetFileNameWithoutExtension(file_path), file_path, tex_target, source);
        }

        /// <summary>
        /// Loads a texture asset from disk into the asset library, with a specific asset key
        /// </summary>
        public static TextureData TextureAssetLoad(string asset_key, string file_path, Silk.NET.OpenGL.TextureTarget tex_target, AssetSource source = AssetSource.adventure)
        {
            file_path = source == AssetSource.engine ? AssetDirectoryEngine + "/Textures/" + file_path : AssetDirectoryAdventure + "/Textures/" + file_path;
            string get_key = AssetLoader.AssetKey(LoadedAsset.AssetType.textures, asset_key, source);
            if (asset_library.TryGetValue(get_key, out LoadedAsset? value)) return (TextureData)value.Data;
            // Check if exists
            if (!File.Exists(file_path))
            {
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=");
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X BAD TEXTURE, FILE DOES NOT EXIST : " + file_path);
                Console.WriteLine("=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=X=");
                return (TextureData)LocateAsset(AssetLoader.AssetKey(LoadedAsset.AssetType.textures, "no_texture", AssetSource.engine)).Data;
            }
            return (TextureData)InvokeAsset(get_key, new AssetTexture(get_key, file_path, tex_target));
        }

        /// <summary>
        /// Creates a material asset and adds it to the asset library for reuse
        /// </summary>
        public static MaterialData MaterialAssetLoad(string asset_key, MaterialData new_material, AssetSource source = AssetSource.adventure)
        {
            string get_key = AssetLoader.AssetKey(LoadedAsset.AssetType.material, asset_key, source);
            if (asset_library.TryGetValue(get_key, out LoadedAsset? value)) return (MaterialData)value.Data;
            return (MaterialData)InvokeAsset(get_key, new AssetMaterial(get_key, new_material));
        }

        /// <summary>
        /// Creates an environment asset and adds it to the asset library for reuse
        /// </summary>
        public static EnvironmentData EnvironmentAssetLoad(EnvironmentData new_environment, AssetSource source = AssetSource.adventure)
        {
            string get_key = AssetLoader.AssetKey(LoadedAsset.AssetType.environment, new_environment.AssetKey, source);
            if (asset_library.TryGetValue(get_key, out LoadedAsset? value)) return (EnvironmentData)value.Data;
            return (EnvironmentData)InvokeAsset(get_key, new AssetEnvironment(get_key, new_environment));
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Assets retrieval
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
        public static LoadedAsset? LocateAsset(string asset_key)
        {
            asset_library.TryGetValue(asset_key, out LoadedAsset found_asset);
            return found_asset;
        }

        /// <summary>
        /// Gets a shader from the asset library
        /// </summary>
        /// 
        public static ShaderData ShaderAssetGet(string asset_key, AssetSource source = AssetSource.adventure)
        {
            LoadedAsset ast = LocateAsset(AssetLoader.AssetKey(LoadedAsset.AssetType.shader, asset_key, source));
            Debug.Assert(ast.CheckType(LoadedAsset.AssetType.shader));
            return (ShaderData)ast.Data;
        }

        /// <summary>
        /// Gets a Model from the asset library
        /// </summary>
        public static ModelData ModelAssetGet(string asset_key, AssetSource source = AssetSource.adventure)
        {
            LoadedAsset ast = LocateAsset(AssetLoader.AssetKey(LoadedAsset.AssetType.model, asset_key, source));
            ast ??= LocateAsset(AssetLoader.AssetKey(LoadedAsset.AssetType.model, "no_model", AssetSource.engine));
            Debug.Assert(ast.CheckType(LoadedAsset.AssetType.model));
            return (ModelData)ast.Data;
        }

        /// <summary>
        /// Gets a texture from the asset library
        /// </summary>
        public static TextureData TextureAssetGet(string asset_key, AssetSource source = AssetSource.adventure)
        {
            LoadedAsset ast = LocateAsset(AssetLoader.AssetKey(LoadedAsset.AssetType.textures, asset_key, source));
            ast ??= LocateAsset(AssetLoader.AssetKey(LoadedAsset.AssetType.textures, "no_texture", AssetSource.engine));
            Debug.Assert(ast.CheckType(LoadedAsset.AssetType.textures));
            return (TextureData)ast.Data;
        }

        /// <summary>
        /// Gets a material from the asset library
        /// </summary>
        public static MaterialData MaterialAssetGet(string asset_key, AssetSource source = AssetSource.adventure)
        {
            LoadedAsset ast = LocateAsset(AssetLoader.AssetKey(LoadedAsset.AssetType.material, asset_key, source));
            ast ??= LocateAsset(AssetLoader.AssetKey(LoadedAsset.AssetType.material, "no_material", AssetSource.engine));
            Debug.Assert(ast.CheckType(LoadedAsset.AssetType.material));
            return (MaterialData)ast.Data;
        }

        /// <summary>
        /// Gets an environment from the asset library
        /// </summary>
        public static EnvironmentData EnvironmentAssetGet(string asset_key, AssetSource source = AssetSource.adventure)
        {
            LoadedAsset ast = LocateAsset(AssetLoader.AssetKey(LoadedAsset.AssetType.environment, asset_key, source));
            ast ??= LocateAsset(AssetLoader.AssetKey(LoadedAsset.AssetType.environment, "standard_day", AssetSource.engine));
            Debug.Assert(ast.CheckType(LoadedAsset.AssetType.environment));
            return (EnvironmentData)ast.Data;
        }
    }
}