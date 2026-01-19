using System.Numerics;
using Assets;

namespace Engine
{
    public partial class Core
    {
        public static MaterialData collision_draw_material;
        public static MaterialData trigger_draw_material;
        public static MaterialData actor_collision_draw_material;

        public static ModelData sprite2d_model;
        public static ModelData collision_model_point;
        public static ModelData collision_model_sphere;
        public static ModelData collision_model_cylinder;
        public static ModelData collision_model_cube;

        public static MaterialData sprite2d_material;

        private void LoadBaseAssets()
        {
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Shaders
            ShaderData shader_standard = AssetLoader.ShaderAssetLoad("standard", AssetLoader.AssetDirectoryEngine + "/Shaders/standard.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/standard.frag", AssetLoader.AssetSource.engine);
            ShaderData shader_unshaded = AssetLoader.ShaderAssetLoad("standard_unshaded", AssetLoader.AssetDirectoryEngine + "/Shaders/unshaded.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/unshaded.frag", AssetLoader.AssetSource.engine);
            ShaderData shader_debugnormals = AssetLoader.ShaderAssetLoad("debug_normals", AssetLoader.AssetDirectoryEngine + "/Shaders/unshaded.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/debug_normal.frag", AssetLoader.AssetSource.engine);
            ShaderData shader_debugcol = AssetLoader.ShaderAssetLoad("debug_col", AssetLoader.AssetDirectoryEngine + "/Shaders/unshaded.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/debug_walls.frag", AssetLoader.AssetSource.engine);
            // skybox
            AssetLoader.ShaderAssetLoad("skybox_daynight_multiblend", AssetLoader.AssetDirectoryEngine + "/Shaders/unshaded.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/skybox_multiblend.frag", AssetLoader.AssetSource.engine);

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Textures
            TextureData no_texture = AssetLoader.TextureAssetLoad(AssetLoader.AssetDirectoryEngine + "/Textures/Error/no_texture.png", Silk.NET.OpenGL.TextureTarget.Texture2D, AssetLoader.AssetSource.engine);
            TextureData nomat_texture = AssetLoader.TextureAssetLoad(AssetLoader.AssetDirectoryEngine + "/Textures/Error/no_material.png", Silk.NET.OpenGL.TextureTarget.Texture2D, AssetLoader.AssetSource.engine);
            TextureData example_texture = AssetLoader.TextureAssetLoad( AssetLoader.AssetDirectoryEngine + "/Textures/Objects/example.png", Silk.NET.OpenGL.TextureTarget.Texture2D, AssetLoader.AssetSource.engine);
            // skybox
            TextureData skybox_tex_dawn = AssetLoader.TextureAssetLoad(AssetLoader.AssetDirectoryEngine + "/Textures/Skybox/standard_skybox_dawn.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            TextureData skybox_tex_day = AssetLoader.TextureAssetLoad(AssetLoader.AssetDirectoryEngine + "/Textures/Skybox/standard_skybox_day.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            TextureData skybox_tex_dusk = AssetLoader.TextureAssetLoad(AssetLoader.AssetDirectoryEngine + "/Textures/Skybox/standard_skybox_dusk.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            TextureData skybox_tex_night = AssetLoader.TextureAssetLoad(AssetLoader.AssetDirectoryEngine + "/Textures/Skybox/standard_skybox_night.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            TextureData skybox_tex_dayrain = AssetLoader.TextureAssetLoad(AssetLoader.AssetDirectoryEngine + "/Textures/Skybox/standard_skybox_dayrain.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            TextureData skybox_tex_nightrain = AssetLoader.TextureAssetLoad(AssetLoader.AssetDirectoryEngine + "/Textures/Skybox/standard_skybox_nightrain.png", Silk.NET.OpenGL.TextureTarget.Texture2D);

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Materials
            AssetLoader.MaterialAssetLoad("debug_normals", new([], [], shader_debugnormals), AssetLoader.AssetSource.engine);
            AssetLoader.MaterialAssetLoad("no_material", new([nomat_texture], [new("uTexture0", 0)], shader_standard), AssetLoader.AssetSource.engine);
            AssetLoader.MaterialAssetLoad("example", new([example_texture], [new("uTexture0", 0)], shader_standard), AssetLoader.AssetSource.engine);
            // debugging
            collision_draw_material = AssetLoader.MaterialAssetLoad("debug_col", new([], [new("uColorSet", new Vector4(0.6f, 0.8f, 0f, 1f))], shader_debugcol), AssetLoader.AssetSource.engine); // Cached in a static for rendering speed reasons
            trigger_draw_material = AssetLoader.MaterialAssetLoad("debug_trigger", new([], [new("uColorSet", new Vector4(0f, 0.2f, 0.9f, 1f))], shader_debugcol), AssetLoader.AssetSource.engine);
            actor_collision_draw_material = AssetLoader.MaterialAssetLoad("debug_actor_col", new([], [new("uColorSet", new Vector4(0.9f, 0.6f, 0f, 1f))], shader_debugcol), AssetLoader.AssetSource.engine); // Cached in a static for rendering speed reasons
            AssetLoader.ModelAssetLoad(AssetLoader.AssetDirectoryEngine + "/Models/pointer.fbx", AssetLoader.AssetSource.engine);
            // effects
            sprite2d_material = AssetLoader.MaterialAssetLoad("sprite2d", new([no_texture], [new("uTexture0", 0)], shader_unshaded), AssetLoader.AssetSource.engine);
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Models: FBX, 0.01 scale, Z forward, Y Up
            // Debug models
            AssetLoader.ModelAssetLoad("no_model", AssetLoader.AssetDirectoryEngine + "/Models/cube.fbx", AssetLoader.AssetSource.engine);
            // Collider visualization
            collision_model_point = AssetLoader.ModelAssetLoad(AssetLoader.AssetDirectoryEngine + "/Models/cube.fbx", AssetLoader.AssetSource.engine);
            collision_model_sphere = AssetLoader.ModelAssetLoad(AssetLoader.AssetDirectoryEngine + "/Models/col_sphere.fbx", AssetLoader.AssetSource.engine);
            collision_model_cylinder = AssetLoader.ModelAssetLoad(AssetLoader.AssetDirectoryEngine + "/Models/col_cylinder.fbx", AssetLoader.AssetSource.engine);
            sprite2d_model = AssetLoader.ModelAssetLoad(AssetLoader.AssetDirectoryEngine + "/Models/sprite2d.fbx", AssetLoader.AssetSource.engine);
            // Misc
            AssetLoader.ModelAssetLoad(AssetLoader.AssetDirectoryEngine + "/Models/cube_map.fbx", AssetLoader.AssetSource.engine);


            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Environment
            EnvironmentData dawn_env = new("standard_dawn", new Vector3(0.25f, 0.46f, 0.79f), 270f, new Vector4(0.9f, 0.85f, 0.85f, 0.9f), skybox_tex_dawn);
            EnvironmentData day_env = new("standard_day", new Vector3(0.25f, 0.46f, 0.79f), 300f, new Vector4(1f, 0.95f, 0.95f, 1f), skybox_tex_day);
            EnvironmentData dusk_env = new("standard_dusk", new Vector3(0.40f, 0.45f, 0.28f), 220f, new Vector4(1f, 1f, 0.85f, 0.9f), skybox_tex_dusk);
            EnvironmentData night_env = new("standard_night", new Vector3(0.7f, 0.5f, 0.14f), 160f, new Vector4(0.5f, 0.5f, 0.5f, 0.3f), skybox_tex_night);
            EnvironmentData rain_day_env = new("standard_rain_day", new Vector3(0.35f, 0.40f, 0.54f), 140f, new Vector4(0.95f, 1f, 0.95f, 0.8f), skybox_tex_dayrain);
            EnvironmentData rain_night_env = new("standard_rain_night", new Vector3(0.13f, 0.13f, 0.13f), 120f, new Vector4(0.4f, 0.6f, 0.5f, 0.25f), skybox_tex_nightrain);

            AssetLoader.EnvironmentAssetLoad(dawn_env, AssetLoader.AssetSource.engine);
            AssetLoader.EnvironmentAssetLoad(day_env, AssetLoader.AssetSource.engine);
            AssetLoader.EnvironmentAssetLoad(dusk_env, AssetLoader.AssetSource.engine);
            AssetLoader.EnvironmentAssetLoad(night_env, AssetLoader.AssetSource.engine);
            // Daynight cycles
            AssetLoader.EnvironmentAssetLoad(new Environments.DayNightCycle("standard_daynight", dawn_env, day_env, dusk_env, night_env, rain_day_env, rain_night_env), AssetLoader.AssetSource.engine);


            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Log all as persistent. Base Assets should not unload.
            AssetLoader.PersistAllAssets();
        }
    }
}