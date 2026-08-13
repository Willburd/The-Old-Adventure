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
    LoadAsset_Texture(ASSET_TEXTURES"/Engine/example.png", TRUE, NULL);
    // Adventure
    LoadAsset_Texture(ASSET_TEXTURES"/Hud/HealthBack.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Hud/HealthQuarter.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Hud/HealthHalf.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Hud/HealthThreeQuarter.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Hud/HealthFull.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Hud/HudButton.png", TRUE, NULL);

    // Load default materials
    LoadAsset_Material(ASSET_MATERIALS"/Engine/no_material.mat", TRUE);
    LoadAsset_Material(ASSET_MATERIALS"/Engine/example.mat", TRUE);
    LoadAsset_Material(ASSET_MATERIALS"/Engine/collision_debug.mat", TRUE);
    // Adventure

    // Load default models
    LoadAsset_Model(CUBEMAP_MODEL, TRUE);
    LoadAsset_Model(QUAD_MODEL, TRUE);
    LoadAsset_Model(SPRITE_MODEL, TRUE);
    LoadAsset_Model(CUBE_MODEL, TRUE);
    // Adventure

    // Set font
    default_font = LoadFontEx(ASSET_FONT"/LinBiolinum_Rah.ttf", 16, NULL, 0); // Adventure edit - Use a different font

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
    // Adventure
    LoadTextData(TEXT_ACTIONBUTTON_READ);
}
