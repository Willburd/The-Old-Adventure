using System.Numerics;
using Rendering;

namespace Engine
{
    public partial class Core 
    {
        public static MaterialData collision_draw_material;
        public static MaterialData trigger_draw_material;
        public static MaterialData actor_collision_draw_material;

        public static ModelData collision_model_point;
        public static ModelData collision_model_sphere;
        public static ModelData collision_model_cylinder;
        public static ModelData collision_model_cube;

        private void LoadBaseAssets()
        {
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Shaders
            ShaderData shader_standard    = AssetLoader.ShaderAssetLoad( "standard", AssetLoader.AssetDirectoryEngine + "/Shaders/standard.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/standard.frag", AssetLoader.AssetSource.engine);
            ShaderData shader_debugnormals= AssetLoader.ShaderAssetLoad( "debug_normals", AssetLoader.AssetDirectoryEngine + "/Shaders/unshaded.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/debug_normal.frag", AssetLoader.AssetSource.engine);
            ShaderData shader_debugcol    = AssetLoader.ShaderAssetLoad( "debug_col", AssetLoader.AssetDirectoryEngine + "/Shaders/unshaded.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/nblend_color.frag", AssetLoader.AssetSource.engine);
                                            AssetLoader.ShaderAssetLoad( "skybox_daynight_multiblend", AssetLoader.AssetDirectoryEngine + "/Shaders/unshaded.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/skybox_multiblend.frag", AssetLoader.AssetSource.engine);


            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Textures
                                            AssetLoader.TextureAssetLoad( "no_tex", AssetLoader.AssetDirectoryEngine + "/Textures/no_texture.png", Silk.NET.OpenGL.TextureTarget.Texture2D, AssetLoader.AssetSource.engine);
            TextureData nomat_texture     = AssetLoader.TextureAssetLoad( "no_mat", AssetLoader.AssetDirectoryEngine + "/Textures/no_material.png", Silk.NET.OpenGL.TextureTarget.Texture2D, AssetLoader.AssetSource.engine);
            TextureData example_texture   = AssetLoader.TextureAssetLoad( "example", AssetLoader.AssetDirectoryEngine + "/Textures/example.png", Silk.NET.OpenGL.TextureTarget.Texture2D, AssetLoader.AssetSource.engine);
            TextureData skybox_tex_dawn   = AssetLoader.TextureAssetLoad( "standard_skybox_dawn", AssetLoader.AssetDirectoryEngine + "/Textures/skybox_dawn.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            TextureData skybox_tex_day    = AssetLoader.TextureAssetLoad( "standard_skybox_day", AssetLoader.AssetDirectoryEngine + "/Textures/skybox_day.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            TextureData skybox_tex_dusk   = AssetLoader.TextureAssetLoad( "standard_skybox_dusk", AssetLoader.AssetDirectoryEngine + "/Textures/skybox_dusk.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            TextureData skybox_tex_night  = AssetLoader.TextureAssetLoad( "standard_skybox_night", AssetLoader.AssetDirectoryEngine + "/Textures/skybox_night.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            TextureData skybox_tex_dayrain   = AssetLoader.TextureAssetLoad( "standard_skybox_rainday", AssetLoader.AssetDirectoryEngine + "/Textures/skybox_dayrain.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            TextureData skybox_tex_nightrain  = AssetLoader.TextureAssetLoad( "standard_skybox_rainnight", AssetLoader.AssetDirectoryEngine + "/Textures/skybox_nightrain.png", Silk.NET.OpenGL.TextureTarget.Texture2D);


            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Materials
            AssetLoader.MaterialAssetLoad( "debug_normals", new( [], [], shader_debugnormals), AssetLoader.AssetSource.engine);
            AssetLoader.MaterialAssetLoad( "no_mat", new( [nomat_texture], [new("uTexture0", 0)], shader_standard), AssetLoader.AssetSource.engine);
            AssetLoader.MaterialAssetLoad( "example", new( [example_texture], [new("uTexture0", 0)], shader_standard), AssetLoader.AssetSource.engine);
            collision_draw_material = AssetLoader.MaterialAssetLoad( "debug_col", new( [], [new("uColorSet", new Vector4(0.6f,0.8f,0f,1f)) ], shader_debugcol), AssetLoader.AssetSource.engine); // Cached in a static for rendering speed reasons
            trigger_draw_material = AssetLoader.MaterialAssetLoad( "debug_trigger", new( [], [new("uColorSet", new Vector4(0f,0.2f,0.9f,1f)) ], shader_debugcol), AssetLoader.AssetSource.engine);
            actor_collision_draw_material = AssetLoader.MaterialAssetLoad( "debug_actor_col", new( [], [new("uColorSet", new Vector4(0.9f,0.6f,0f,1f)) ], shader_debugcol), AssetLoader.AssetSource.engine); // Cached in a static for rendering speed reasons


            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Models: FBX, 0.01 scale, Z forward, Y Up
            // Debug models
            AssetLoader.ModelAssetLoad( "no_model", AssetLoader.AssetDirectoryEngine + "/Models/cube.fbx", AssetLoader.AssetSource.engine); 
            // Collider visualization
            collision_model_point = AssetLoader.ModelAssetLoad( "cube", AssetLoader.AssetDirectoryEngine + "/Models/cube.fbx", AssetLoader.AssetSource.engine); 
            collision_model_sphere = AssetLoader.ModelAssetLoad( "col_sphere", AssetLoader.AssetDirectoryEngine + "/Models/col_sphere.fbx", AssetLoader.AssetSource.engine); 
            collision_model_cylinder = AssetLoader.ModelAssetLoad( "col_cylinder", AssetLoader.AssetDirectoryEngine + "/Models/col_cylinder.fbx", AssetLoader.AssetSource.engine); 
            // Misc
            AssetLoader.ModelAssetLoad( "cube_map", AssetLoader.AssetDirectoryEngine + "/Models/cube_map.fbx", AssetLoader.AssetSource.engine); 


            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Environment
            Environments.Environment dawn_env = new("standard_dawn", new Vector4( 0.25f, 0.46f, 0.79f, 1f), 270f, new Vector4(0.9f,0.85f,0.85f,0.9f), skybox_tex_dawn);
            Environments.Environment day_env = new("standard_day", new Vector4( 0.25f, 0.46f, 0.79f, 1f), 300f, new Vector4(1f,0.95f,0.95f,1f), skybox_tex_day);
            Environments.Environment dusk_env = new("standard_dusk", new Vector4( 0.40f, 0.45f, 0.28f, 1f), 220f, new Vector4(1f,1f,0.85f,0.9f), skybox_tex_dusk);
            Environments.Environment night_env = new("standard_night", new Vector4( 0.7f, 0.5f, 0.14f, 1f), 160f, new Vector4(0.5f,0.5f,0.5f,0.3f), skybox_tex_night);
            Environments.Environment rain_day_env = new("standard_rain_day", new Vector4( 0.35f, 0.40f, 0.54f, 1f), 140f, new Vector4(0.95f,1f,0.95f,0.8f), skybox_tex_dayrain);
            Environments.Environment rain_night_env = new("standard_rain_night", new Vector4( 0.13f, 0.13f, 0.13f, 1f), 120f, new Vector4(0.4f,0.6f,0.5f,0.25f), skybox_tex_nightrain);

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