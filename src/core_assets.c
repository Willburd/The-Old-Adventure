#include "core_assets.h"
#include "text_loading.h"

// Loads all core assets, and flags them as core assets.
void LoadCoreAssets()
{
    printf("==============================================================================\n");
    printf("                             LOADING CORE ASSETS                              \n");
    printf("==============================================================================\n");

    // Load default textures
    LoadAsset_Texture(ASSET_TEXTURES"/Engine/no_texture.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Engine/no_material.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Engine/white.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Engine/black.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Engine/example.png", TRUE, NULL);
    // TODO - Your default textures here

    // Load default materials
    LoadAsset_Material(ASSET_MATERIALS"/Engine/no_material.mat", TRUE);
    LoadAsset_Material(ASSET_MATERIALS"/Engine/example.mat", TRUE);
    LoadAsset_Material(ASSET_MATERIALS"/Engine/black.mat", TRUE);
    LoadAsset_Material(ASSET_MATERIALS"/Engine/white.mat", TRUE);
    LoadAsset_Material(ASSET_MATERIALS"/Engine/color_blend.mat", TRUE);
    LoadAsset_Material(ASSET_MATERIALS"/Engine/collision_debug.mat", TRUE);
    // TODO - Your default materials here

    // Load default models
    LoadAsset_Model(CUBEMAP_MODEL, TRUE);
    LoadAsset_Model(QUAD_MODEL, TRUE);
    LoadAsset_Model(SPRITE_MODEL, TRUE);
    LoadAsset_Model(CUBE_MODEL, TRUE);
    // TODO - Your default models here

    // Set font
    default_font = LoadFontEx(ASSET_FONT"/PressStart2P-Regular.ttf", 16, NULL, 0); // TODO - Your font here
    
    // Load text
    LoadCoreTextAssets();

    printf("==============================================================================\n");
    printf("                             FINISHED CORE ASSETS                             \n");
    printf("==============================================================================\n");
}

void LoadCoreTextAssets()
{
    current_game_language = lang_EN;
    LoadTextData(TEXT_TEST_DEBUG);
}