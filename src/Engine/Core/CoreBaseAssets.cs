using Rendering;

namespace Engine
{
    public partial class Core 
    {
        public static MaterialData collision_draw_material;
        public static MaterialData actor_collision_draw_material;

        public static ModelData collision_model_sphere;
        public static ModelData collision_model_cylinder;
        public static ModelData collision_model_cube;

        private void LoadBaseAssets()
        {
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Shaders
            ShaderData shader_standard      = AssetLoader.ShaderAssetLoad( "standard", AssetLoader.AssetDirectoryEngine + "/Shaders/standard.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/standard.frag", AssetLoader.AssetSource.engine);
            ShaderData shader_debugnormals  = AssetLoader.ShaderAssetLoad( "debug_normals", AssetLoader.AssetDirectoryEngine + "/Shaders/standard.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/debug_normal.frag", AssetLoader.AssetSource.engine);
            ShaderData shader_debugcol      = AssetLoader.ShaderAssetLoad( "debug_col", AssetLoader.AssetDirectoryEngine + "/Shaders/standard.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/debug_col.frag", AssetLoader.AssetSource.engine);
            ShaderData shader_debugactorcol = AssetLoader.ShaderAssetLoad( "debug_actor_col", AssetLoader.AssetDirectoryEngine + "/Shaders/standard.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/debug_actor_col.frag", AssetLoader.AssetSource.engine);


            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Textures
                                            AssetLoader.TextureAssetLoad( "no_tex", AssetLoader.AssetDirectoryEngine + "/Textures/no_texture.png", AssetLoader.AssetSource.engine);
            TextureData nomat_texture     = AssetLoader.TextureAssetLoad( "no_mat", AssetLoader.AssetDirectoryEngine + "/Textures/no_material.png", AssetLoader.AssetSource.engine);
            TextureData example_texture   = AssetLoader.TextureAssetLoad( "example", AssetLoader.AssetDirectoryEngine + "/Textures/example.png", AssetLoader.AssetSource.engine);


            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Materials
            AssetLoader.MaterialAssetLoad( "debug_normals", new( [], [], shader_debugnormals), AssetLoader.AssetSource.engine);
            AssetLoader.MaterialAssetLoad( "no_mat", new( [nomat_texture], [new MaterialUniformData("uTexture0", 0)], shader_standard), AssetLoader.AssetSource.engine);
            AssetLoader.MaterialAssetLoad( "example", new( [example_texture], [new MaterialUniformData("uTexture0", 0)], shader_standard), AssetLoader.AssetSource.engine);
            collision_draw_material = AssetLoader.MaterialAssetLoad( "debug_col", new( [], [], shader_debugcol), AssetLoader.AssetSource.engine); // Cached in a static for rendering speed reasons
            actor_collision_draw_material = AssetLoader.MaterialAssetLoad( "debug_actor_col", new( [], [], shader_debugactorcol), AssetLoader.AssetSource.engine); // Cached in a static for rendering speed reasons


            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Models: FBX, 0.01 scale, Z forward, Y Up
            // Debug models
            AssetLoader.ModelAssetLoad( "no_model", AssetLoader.AssetDirectoryEngine + "/Models/cube.fbx", AssetLoader.AssetSource.engine); 
            AssetLoader.ModelAssetLoad( "cube", AssetLoader.AssetDirectoryEngine + "/Models/cube.fbx", AssetLoader.AssetSource.engine); 
            // Collider visualization
            collision_model_sphere = AssetLoader.ModelAssetLoad( "col_sphere", AssetLoader.AssetDirectoryEngine + "/Models/col_shere.fbx", AssetLoader.AssetSource.engine); 
            collision_model_cylinder = AssetLoader.ModelAssetLoad( "col_cylinder", AssetLoader.AssetDirectoryEngine + "/Models/col_cylinder.fbx", AssetLoader.AssetSource.engine); 


            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Log all as persistent. Base Assets should not unload.
            AssetLoader.PersistAllAssets();
        }
    }
}