namespace Engine
{
    public class Asset(Object new_asset)
    {
        protected readonly Object data = new_asset;
        protected bool always_loaded = false;

        public void SetPersistent()
        {
            always_loaded = true;
        }

        public Object GetAsset 
        {
            get
            {
                return data;
            }
        }

        public bool Persistent
        {
            get
            {
                return always_loaded;
            }
        }

        public virtual void Cleanup()
        {
            
        }
    }
}