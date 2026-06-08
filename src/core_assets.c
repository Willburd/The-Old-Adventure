#include "core_assets.h"

// Loads all core assets, and flags them as core assets.
void LoadCoreAssets()
{
    // Load default textures
    LoadAsset_Texture(ASSET_TEXTURES"/Error/no_texture.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Error/no_material.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Objects/example.png", TRUE, NULL);

    // Load default materials
    LoadAsset_Material(ASSET_MATERIALS"/Error/no_material.mat", TRUE);
    LoadAsset_Material(ASSET_MATERIALS"/Objects/example.mat", TRUE);
    LoadAsset_Material(SKYBOX_MATERIAL_CYCLE, TRUE);

    // Load default models
    LoadAsset_Model(CUBEMAP_MODEL, TRUE);
    LoadAsset_Model(SKYSPHERE_MODEL, TRUE);
}