using System.Numerics;
using Engine;
using EntComponents;
using Assets;

namespace Environments
{
    public class DayNightCycle : EnvironmentData
    {
        public DayNightCycle(string asset_key, EnvironmentData env_dawn, EnvironmentData env_day, EnvironmentData env_dusk, EnvironmentData env_night, EnvironmentData env_rainday, EnvironmentData env_rainnight) : base(asset_key, Vector3.Zero, 0f, Vector4.Zero)
        {
            dawn = env_dawn;
            day = env_day;
            dusk = env_dusk;
            night = env_night;
            rainday = env_rainday;
            rainnight = env_rainnight;

            // Multiblend skybox behavior
            multiblend_skybox_mat = AssetLoader.MaterialAssetLoad(asset_key
                                                                , new([dawn.SkyboxTexture, day.SkyboxTexture, dusk.SkyboxTexture, night.SkyboxTexture, rainday.SkyboxTexture, rainnight.SkyboxTexture]
                                                                , [new("uTextureDawn", 0), new("uTextureDay", 1), new("uTextureDusk", 2), new("uTextureNight", 3), new("uTextureDayRain", 4), new("uTextureNightRain", 5), new("uDuskPerc", cycle), new("uNightPerc", cycle), new("uDawnPerc", cycle), new("uRainPerc", rain_intensity)]
                                                                , AssetLoader.ShaderAssetGet("skybox_daynight_multiblend", AssetLoader.AssetSource.engine)));
        }

        private EnvironmentData dawn;
        private EnvironmentData day;
        private EnvironmentData night;
        private EnvironmentData dusk;
        private EnvironmentData rainday;
        private EnvironmentData rainnight;
        private MaterialData multiblend_skybox_mat;

        private float cycle = 0f;
        public float rain_intensity = 0f;

        public override void ApplyEnvironment(Room host_room)
        {
            // Set room
            Host = host_room;
            skybox_model = new Skybox(Host);
            skybox_model.SetModel(AssetLoader.ModelAssetGet("cube_map", AssetLoader.AssetSource.engine), multiblend_skybox_mat);
        }

        public override void Update()
        {
            cycle += 0.001f;
            float cycle_mod = cycle % 1;
            float dusk_intensity = Math.Clamp(MathF.Pow(MathF.Sin(cycle_mod * MathF.PI), 15f) * 1.1f, 0f, 1f);
            float night_intensity = Math.Clamp(-MathF.Sin(cycle_mod * (MathF.PI * 2)) * 1.5f, 0f, 1f);
            float dawn_intensity = Math.Clamp(MathF.Pow(MathF.Sin((cycle_mod * MathF.PI) + (MathF.PI * 0.5f)), 20f) * 1.1f, 0f, 1f);

            MaterialData skymat = skybox_model.GetMaterial(0);
            if (skymat == null) return;
            for (int i = 0; i < skymat.Uniforms.Count; i++)
            {
                ShaderData.Uniform uniform = skymat.Uniforms[i];
                switch (uniform.key)
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

                    case "uRainPerc":
                        uniform.value = rain_intensity;
                        break;
                }
                // reassign
                skymat.Uniforms[i] = uniform;
            }

            FogDistance = float.Lerp(day.FogDistance, rainday.FogDistance, rain_intensity);
            FogDistance = float.Lerp(FogDistance, float.Lerp(dusk.FogDistance, rainday.FogDistance, rain_intensity), dusk_intensity);
            FogDistance = float.Lerp(FogDistance, float.Lerp(night.FogDistance, rainnight.FogDistance, rain_intensity), night_intensity);
            FogDistance = float.Lerp(FogDistance, float.Lerp(dawn.FogDistance, rainday.FogDistance, rain_intensity), dawn_intensity);

            FogColor = Vector3.Lerp(day.FogColor, rainday.FogColor, rain_intensity);
            FogColor = Vector3.Lerp(FogColor, Vector3.Lerp(dusk.FogColor, rainday.FogColor, rain_intensity), dusk_intensity);
            FogColor = Vector3.Lerp(FogColor, Vector3.Lerp(night.FogColor, rainnight.FogColor, rain_intensity), night_intensity);
            FogColor = Vector3.Lerp(FogColor, Vector3.Lerp(dawn.FogColor, rainday.FogColor, rain_intensity), dawn_intensity);

            AmbientLight = Vector4.Lerp(day.AmbientLight, rainday.AmbientLight, rain_intensity);
            AmbientLight = Vector4.Lerp(AmbientLight, Vector4.Lerp(dusk.AmbientLight, rainday.AmbientLight, rain_intensity), dusk_intensity);
            AmbientLight = Vector4.Lerp(AmbientLight, Vector4.Lerp(night.AmbientLight, rainnight.AmbientLight, rain_intensity), night_intensity);
            AmbientLight = Vector4.Lerp(AmbientLight, Vector4.Lerp(dawn.AmbientLight, rainday.AmbientLight, rain_intensity), dawn_intensity);
        }

        public override bool IsValid()
        {
            if (!dawn.IsValid()) return false;
            if (!day.IsValid()) return false;
            if (!dusk.IsValid()) return false;
            if (!night.IsValid()) return false;
            return true;
        }
    }
}