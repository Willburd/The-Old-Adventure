namespace Engine
{
    /// <summary>
    /// Base asset class, handles loading and unloading of data read from files. Can be marked persistent to prevent deletion during basic unloads like scene transitions.
    /// </summary>
    public class Asset
    {
#pragma warning disable CS8618 // Were the base type, all our children set us...
        /// <summary>
        /// The stored asset itself. Stored as a generic object. The subtype of this object returns the correct type when GetAsset is called.
        /// </summary>
        protected Object data;

        public Asset(string key, string path)
        {
            asset_key = key;
            if(path != "") file_path = path;
            Console.WriteLine(GetType() + " > " + asset_key + " : " + file_path);
        }
#pragma warning restore CS8618

        /// <summary>
        /// Asset key, mostly for debugging.
        /// </summary>
        protected string asset_key;

        /// <summary>
        /// Controls if an asset is unloaded during scene transitions. Otherwise only the game shutting down, or otherwise force unloading will unload the asset.
        /// </summary>
        protected bool always_loaded = false;

        /// <summary>
        /// Stores the file path for debugging information
        /// </summary>
        protected string file_path = "NO FILE";

        /// <summary>
        /// Flags an asset to no longer unload during basic cleanup, such as scene transitions.
        /// </summary>
        public void SetPersistent()
        {
            always_loaded = true;
        }

        /// <summary>
        /// Gets the stored asset for use.
        /// </summary>
        public Object Data
        {
            get
            {
                return data;
            }
        }

        /// <summary>
        /// Checks if an asset shouldn't be unloaded during basic asset cleaning, such as scene transitions.
        /// </summary>
        public bool Persistent
        {
            get
            {
                return always_loaded;
            }
        }

        public string FilePath
        {
            get
            {
                return file_path;
            }
        }

        /// <summary>
        /// Frees the asset's data during unloading or reloading.
        /// </summary>
        public virtual void Unload()
        {
            Console.WriteLine(GetType() + " X " + asset_key + " : " + file_path);
        }
        
        /// <summary>
        /// Checks if an asset loaded correctly.
        /// </summary>
        public virtual bool CheckIntegrity()
        {
            return false;
        }
    }
}