using Rendering;

namespace Engine
{
    public partial class Core 
    {
        private void LoadBaseAssets()
        {
            // Shaders
            AssetLoader.ShaderAssetLoad( AssetLoader.AssetKey(Asset.AssetType.shader, "standard"), AssetLoader.AssetDirectoryEngine + "/Shaders/standard.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/standard.frag");
            ShaderData debug_normals_shader = AssetLoader.ShaderAssetLoad( AssetLoader.AssetKey(Asset.AssetType.shader, "debug_normals"), AssetLoader.AssetDirectoryEngine + "/Shaders/standard.vert", AssetLoader.AssetDirectoryEngine + "/Shaders/debug_normal.frag");

            // Textures


            // Materials
            AssetLoader.MaterialAssetLoad( AssetLoader.AssetKey(Asset.AssetType.material, "debug_normals"), new( [], [], debug_normals_shader));

            // Log all as persistent. Base Assets should not unload.
            AssetLoader.PersistAllAssets();
        }
    }
}