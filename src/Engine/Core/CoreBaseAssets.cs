using Rendering;

namespace Engine
{
    public partial class Core 
    {
        private void LoadBaseAssets()
        {
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Shaders
            ShaderData shader_standard      = AssetLoader.ShaderAssetLoad( "standard", AssetLoader.AssetDirectoryEngine + "/Shaders/standard.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/standard.frag", AssetLoader.AssetSource.engine);
            ShaderData shader_debugnormals  = AssetLoader.ShaderAssetLoad( "debug_normals", AssetLoader.AssetDirectoryEngine + "/Shaders/standard.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/debug_normal.frag", AssetLoader.AssetSource.engine);
            ShaderData shader_debugcol      = AssetLoader.ShaderAssetLoad( "debug_col", AssetLoader.AssetDirectoryEngine + "/Shaders/standard.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/debug_col.frag", AssetLoader.AssetSource.engine);


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


            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Models: FBX, 0.01 scale, Z forward, Y Up
            AssetLoader.ModelAssetLoad( "no_model", AssetLoader.AssetDirectoryEngine + "/Models/cube.fbx", AssetLoader.AssetSource.engine); 
            AssetLoader.ModelAssetLoad( "cube", AssetLoader.AssetDirectoryEngine + "/Models/cube.fbx", AssetLoader.AssetSource.engine); 


            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Log all as persistent. Base Assets should not unload.
            AssetLoader.PersistAllAssets();
        }
    }
}