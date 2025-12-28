using Rendering;

namespace Engine
{
    public partial class Core 
    {
        private void LoadBaseAssets()
        {
            // Shaders
            ShaderData shader_standard      = AssetLoader.ShaderAssetLoad( AssetLoader.AssetKey(Asset.AssetType.shader, "standard", AssetLoader.AssetSource.engine), 
                                                                            AssetLoader.AssetDirectoryEngine + "/Shaders/standard.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/standard.frag");
            ShaderData shader_debugnormals  = AssetLoader.ShaderAssetLoad( AssetLoader.AssetKey(Asset.AssetType.shader, "debug_normals", AssetLoader.AssetSource.engine), 
                                                                            AssetLoader.AssetDirectoryEngine + "/Shaders/standard.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/debug_normal.frag");

            // Textures


            // Materials
            AssetLoader.MaterialAssetLoad( AssetLoader.AssetKey(Asset.AssetType.material, "debug_normals", AssetLoader.AssetSource.engine),
                                            new( [], [], shader_debugnormals));

            // Models: FBX, 1 scale, Y up, X forward
            AssetLoader.ModelAssetLoad( AssetLoader.AssetKey(Asset.AssetType.model, "cube", AssetLoader.AssetSource.engine) , 
                                            AssetLoader.AssetDirectoryAdventure + "/Models/test.fbx"); 

            // Log all as persistent. Base Assets should not unload.
            AssetLoader.PersistAllAssets();
        }
    }
}