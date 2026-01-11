using System.Drawing;
using System.Numerics;
using Engine;
using EntComponents;
using Rendering;
using Silk.NET.OpenGL;

namespace Environments
{
    public class DayNightCycle : Environment
    {
        public DayNightCycle(string asset_key, Environment env_dawn, Environment env_day, Environment env_dusk, Environment env_night) : base(asset_key, Vector4.Zero, 0f, Vector4.Zero)
        {
            dawn = env_dawn;
            day = env_day;
            dusk = env_dusk;
            night = env_night;

            // Multiblend skybox behavior
            multiblend_skybox_mat = AssetLoader.MaterialAssetLoad(asset_key
                                                                , new( [dawn.SkyboxTexture , day.SkyboxTexture  , dusk.SkyboxTexture, night.SkyboxTexture]
                                                                ,      [new("uTextureDawn", 0), new("uTextureDay", 1), new("uTextureDusk", 2), new("uTextureNight", 3), new("uDuskPerc", cycle), new("uNightPerc", cycle), new("uDawnPerc", cycle)]
                                                                ,      AssetLoader.ShaderAssetGet("skybox_daynight_multiblend", AssetLoader.AssetSource.engine)));
        }

        private Environment dawn;
        private Environment day;
        private Environment night;
        private Environment dusk;
        private MaterialData multiblend_skybox_mat;

        private float cycle = 0f;

        public override void ApplyEnvironment(Room host_room)
        {
            // Set room
            Host = host_room;
            skybox_model = new Skybox(Host);
            skybox_model.SetModel( AssetLoader.ModelAssetGet("cube_map", AssetLoader.AssetSource.engine), multiblend_skybox_mat);
        }

        public override void Update()
        {
            cycle += 0.001f;
            float cycle_mod = cycle % 1;
            float dusk_intensity = Math.Clamp(MathF.Pow(MathF.Sin(cycle_mod * MathF.PI), 15f) * 1.1f, 0f,1f);
            float night_intensity = Math.Clamp(-MathF.Sin(cycle_mod * (MathF.PI * 2)) * 1.5f, 0f,1f);
            float dawn_intensity = Math.Clamp( MathF.Pow(MathF.Sin((cycle_mod * MathF.PI) + (MathF.PI * 0.5f)), 20f) * 1.1f, 0f,1f);

            MaterialData? skymat = skybox_model?.GetMaterial(0);
            if(skymat == null) return;
            for(int i = 0; i < skymat.Uniforms.Count; i++)
            {
                ShaderData.Uniform uniform = skymat.Uniforms[i];
                switch(uniform.key)
                {
                    case "uDuskPerc":
                        uniform.value = dusk_intensity;
                    break;

                    case "uNightPerc":
                        uniform.value = night_intensity;
                    break;

                    case "uDawnPerc":
                        uniform.value = dawn_intensity;
                    break;
                }
                // reassign
                skymat.Uniforms[i] = uniform;
            }

            FogDistance = day.FogDistance;
            FogDistance = float.Lerp(FogDistance, dusk.FogDistance, dusk_intensity);
            FogDistance = float.Lerp(FogDistance, night.FogDistance, night_intensity);
            FogDistance = float.Lerp(FogDistance, dawn.FogDistance, dawn_intensity);

            FogColor = day.FogColor;
            FogColor = Vector4.Lerp(FogColor, dusk.FogColor, dusk_intensity);
            FogColor = Vector4.Lerp(FogColor, night.FogColor, night_intensity);
            FogColor = Vector4.Lerp(FogColor, dawn.FogColor, dawn_intensity);

            AmbientLight = day.AmbientLight;
            AmbientLight = Vector4.Lerp(AmbientLight, dusk.AmbientLight, dusk_intensity);
            AmbientLight = Vector4.Lerp(AmbientLight, night.AmbientLight, night_intensity);
            AmbientLight = Vector4.Lerp(AmbientLight, dawn.AmbientLight, dawn_intensity);
        }

        public override bool IsValid()
        {
            if(!dawn.IsValid()) return false;
            if(!day.IsValid()) return false;
            if(!dusk.IsValid()) return false;
            if(!night.IsValid()) return false;
            return true;
        }
    }
}