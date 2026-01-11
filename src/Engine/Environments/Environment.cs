using EntComponents;
using System.Numerics;
using Engine;
using Silk.NET.Assimp;
using Rendering;

namespace Environments
{
    public class Environment
    {
        public Room Host { get; protected set; }
        public float FogDistance { get; protected set; }
        public Vector4 FogColor { get; protected set; }
        public Vector4 AmbientLight { get; protected set; }
        public MaterialData? SkyboxMaterial { get; protected set; }
        public TextureData? SkyboxTexture { get; protected set; }

        protected Skybox? skybox_model = null;

        public Environment(Room host, Vector4 fog_col, float fog_dist, Vector4 ambient_light, TextureData? skybox_tex)
        {
            Host = host;
            FogDistance = fog_dist;
            FogColor = fog_col;
            AmbientLight = ambient_light;
            SkyboxTexture = skybox_tex;
        }

        public Environment(Room host, Vector4 fog_col, float fog_dist, Vector4 ambient_light, MaterialData? skybox_mat)
        {
            Host = host;
            FogDistance = fog_dist;
            FogColor = fog_col;
            AmbientLight = ambient_light;
            SkyboxMaterial = skybox_mat;
        }
        
        public Environment(Room host, Vector4 fog_col, float fog_dist, Vector4 ambient_light)
        {
            Host = host;
            FogDistance = fog_dist;
            FogColor = fog_col;
            AmbientLight = ambient_light;
        }


        public virtual void ApplyEnvironment()
        {
            // Default skybox behavior
            if(SkyboxTexture != null)
            {
                // Assemble from texture
                ShaderData standard_shader = AssetLoader.ShaderAssetGet("standard", AssetLoader.AssetSource.engine);
                skybox_model = new Skybox(Host);
                SkyboxMaterial = AssetLoader.MaterialAssetLoad( "testroom_skybox", new( [SkyboxTexture], [new("uTexture0", 0)], standard_shader));
                skybox_model.SetModel( AssetLoader.ModelAssetGet("cube_map", AssetLoader.AssetSource.engine), SkyboxMaterial);
                return;
            }
            if(SkyboxMaterial != null)
            {
                // Assemble from material
                skybox_model = new Skybox(Host);
                skybox_model.SetModel( AssetLoader.ModelAssetGet("cube_map", AssetLoader.AssetSource.engine), SkyboxMaterial);
                SkyboxTexture = SkyboxMaterial.Textures[0];
                return;
            }
        }

        public virtual void Update() { }
    }
}