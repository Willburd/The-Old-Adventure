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
        public DayNightCycle(Room host, Environment env_day, Environment env_dusk, Environment env_night, Environment env_dawn) : base(host, Vector4.Zero, 0f, Vector4.Zero)
        {
            dawn = env_dawn;
            day = env_day;
            dusk = env_dusk;
            night = env_night;
        }

        private Environment dawn;
        private Environment day;
        private Environment night;
        private Environment dusk;

        private float cycle = 0f;

        public override void ApplyEnvironment()
        {
            // Multiblend skybox behavior
            MaterialData multiblend_skybox_mat = AssetLoader.MaterialAssetLoad( "skybox_daynight_multiblend"
                                                                                , new( [dawn.SkyboxTexture , day.SkyboxTexture  , dusk.SkyboxTexture, night.SkyboxTexture]
                                                                                ,      [new("uTextureDawn", 0), new("uTextureDay", 1), new("uTextureDusk", 2), new("uTextureNight", 3), new("uDuskPerc", cycle), new("uNightPerc", cycle), new("uDawnPerc", cycle)]
                                                                                ,      AssetLoader.ShaderAssetGet("skybox_daynight_multiblend", AssetLoader.AssetSource.engine)));
            skybox_model = new Skybox(Host);
            skybox_model.SetModel( AssetLoader.ModelAssetGet("cube_map", AssetLoader.AssetSource.engine), multiblend_skybox_mat);
        }

        public override void Update()
        {
            cycle += 0.001f;
            float dusk_intensity = Math.Clamp( MathF.Pow(MathF.Sin((cycle * MathF.PI) + (MathF.PI * 0.5f)), 20f) * 1.1f, 0f,1f);
            float night_intensity = Math.Clamp(-MathF.Sin(cycle * (MathF.PI * 2)) * 1.2f, 0f,1f);
            float dawn_intensity = Math.Clamp(MathF.Pow(MathF.Sin(cycle * MathF.PI), 10f) * 1.1f, 0f,1f);

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
            }
            

            FogDistance = day.FogDistance;

            FogColor = day.FogColor;

            AmbientLight = day.AmbientLight;
        }
    }
}