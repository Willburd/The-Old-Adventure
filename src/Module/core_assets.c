#include "core_assets.h"
#include "../text_loading.h"

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
    // Adventure
    LoadAsset_Texture(ASSET_TEXTURES"/Hud/HealthBack.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Hud/HealthQuarter.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Hud/HealthHalf.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Hud/HealthThreeQuarter.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Hud/HealthFull.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/Hud/HudButton.png", TRUE, NULL);
    //Luts
    LoadAsset_Texture(ASSET_TEXTURES"/LUTs/BlowOut.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/LUTs/ColdInside.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/LUTs/Foggy.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/LUTs/Meltdown.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/LUTs/Mono.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/LUTs/Neutral.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/LUTs/Night.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/LUTs/Outside.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/LUTs/Sepia.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/LUTs/Shaded.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/LUTs/Sickening.png", TRUE, NULL);
    LoadAsset_Texture(ASSET_TEXTURES"/LUTs/UnderWater.png", TRUE, NULL);
    
    // Load default materials
    LoadAsset_Material(ASSET_MATERIALS"/Engine/no_material.mat", TRUE);
    LoadAsset_Material(ASSET_MATERIALS"/Engine/example.mat", TRUE);
    LoadAsset_Material(ASSET_MATERIALS"/Engine/black.mat", TRUE);
    LoadAsset_Material(ASSET_MATERIALS"/Engine/white.mat", TRUE);
    LoadAsset_Material(ASSET_MATERIALS"/Engine/color_blend.mat", TRUE);
    LoadAsset_Material(ASSET_MATERIALS"/Engine/collision_debug.mat", TRUE);
    // Adventure
    LoadAsset_Material(ASSET_MATERIALS"/LUTs/neutral.mat", TRUE);
    LoadAsset_Material(ASSET_MATERIALS"/Effects/dither.mat", TRUE);

    // Load default models
    LoadAsset_Model(CUBEMAP_MODEL, TRUE);
    LoadAsset_Model(QUAD_MODEL, TRUE);
    LoadAsset_Model(SPRITE_MODEL, TRUE);
    LoadAsset_Model(CUBE_MODEL, TRUE);
    // Adventure
    LoadAsset_Model(SIMPLE_ACTOR_COLLISION_MODEL, TRUE);

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
