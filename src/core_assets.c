#include "core_assets.h"

// Loads all core assets, and flags them as core assets.
void LoadCoreAssets()
{
    // Read materials list

    // Load the default assets
    LoadAsset_Texture(ASSET_TEXTURES"/Error/no_texture.png", TRUE);
    LoadAsset_Texture(ASSET_TEXTURES"/Error/no_material.png", TRUE);
}