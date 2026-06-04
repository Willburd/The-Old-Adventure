#include "core_assets.h"

// Loads all core assets, and flags them as core assets.
void LoadCoreAssets()
{
    // Load default textures
    LoadAsset_Texture(ASSET_TEXTURES"/Error/no_texture.png", TRUE);
    LoadAsset_Texture(ASSET_TEXTURES"/Error/no_material.png", TRUE);

    // Load default materials
    LoadAsset_Material(ASSET_MATERIALS"/Error/no_material.mat", TRUE);

}