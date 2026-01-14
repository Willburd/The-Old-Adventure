using EntComponents;
using System.Numerics;
using Engine;
using Assets;

namespace Assets
{
    public class EnvironmentData
    {
        public Room? Host { get; protected set; }
        public float FogDistance { get; protected set; }
        public Vector4 FogColor { get; protected set; }
        public Vector4 AmbientLight { get; protected set; }
        public MaterialData? SkyboxMaterial { get; protected set; }
        public TextureData? SkyboxTexture { get; protected set; }

        public string AssetKey { get; private set; } 

        protected Skybox? skybox_model = null;

        public EnvironmentData(string asset_key, Vector4 fog_col, float fog_dist, Vector4 ambient_light, TextureData skybox_tex)
        {
            AssetKey = asset_key;
            Host = null;
            
            FogDistance = fog_dist;
            FogColor = fog_col;
            AmbientLight = ambient_light;

            SkyboxTexture = skybox_tex;
            SkyboxMaterial = AssetLoader.MaterialAssetLoad( "testroom_skybox", new( [SkyboxTexture], [new("uTexture0", 0)], AssetLoader.ShaderAssetGet("standard", AssetLoader.AssetSource.engine)));
        }

        public EnvironmentData(string asset_key, Vector4 fog_col, float fog_dist, Vector4 ambient_light, MaterialData skybox_mat)
        {
            AssetKey = asset_key;
            Host = null;

            FogDistance = fog_dist;
            FogColor = fog_col;
            AmbientLight = ambient_light;

            SkyboxMaterial = skybox_mat;
            SkyboxTexture = SkyboxMaterial.Textures[0];
        }
        
        public EnvironmentData(string asset_key, Vector4 fog_col, float fog_dist, Vector4 ambient_light)
        {
            AssetKey = asset_key;
            Host = null;

            FogDistance = fog_dist;
            FogColor = fog_col;
            AmbientLight = ambient_light;

            SkyboxMaterial = null;
            SkyboxTexture = null;
        }


        public virtual void ApplyEnvironment(Room host_room)
        {
            // Set room
            Host = host_room;
            skybox_model = new Skybox(Host);
            skybox_model.SetModel( AssetLoader.ModelAssetGet("cube_map", AssetLoader.AssetSource.engine), SkyboxMaterial);
            return;
        }

        public void Unload()
        {
            Host = null;
        }

        public virtual void Update() { }

        public virtual bool IsValid()
        {
            if(SkyboxMaterial == null) return false;
            return SkyboxMaterial.IsValid();
        }
    }
}