#include "core_assets.h"

// Loads all core assets, and flags them as core assets.
void LoadCoreAssets()
{
    // Load default textures
    LoadAsset_Texture(ASSET_TEXTURES"/Error/no_texture.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Error/no_material.png", TRUE, NULL);

    // Load default materials
    LoadAsset_Material(ASSET_MATERIALS"/Error/no_material.mat", TRUE);

    LoadAsset_Material(SKYBOX_MATERIAL_DAWN, TRUE);
    LoadAsset_Material(SKYBOX_MATERIAL_DAYRAIN, TRUE);
    LoadAsset_Material(SKYBOX_MATERIAL_DUSK, TRUE);
    LoadAsset_Material(SKYBOX_MATERIAL_NIGHT, TRUE);
    LoadAsset_Material(SKYBOX_MATERIAL_NIGHTRAIN, TRUE);
    LoadAsset_Material(SKYBOX_MATERIAL_CYCLE, TRUE);

    // Load default models
    LoadAsset_Model(CUBEMAP_MODEL, TRUE);
}