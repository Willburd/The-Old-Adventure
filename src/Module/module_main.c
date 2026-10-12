#include "module_main.h"
#include "world_state.h"
#include "inventory.h"
#include "../assets.h"
#include "adv_postprocessing.h"

void ModuleInit()
{
	MALLOC_SET(Inventory, player_inventory, NULL);
	InitWorldState();

    RegisterWorldPostProcessShader(
        AssetGet_Material(ASSET_MATERIALS"/Effects/dither.mat"), "WorldDither", 10, AdvDitherShaderUniforms);
    RegisterWorldPostProcessShader(
        AssetGet_Material(ASSET_MATERIALS"/Effects/look_up_table.mat"), "WorldLUT", 20, AdvLUTShaderUniforms);
    RegisterWorldPostProcessShader(
        AssetGet_Material(ASSET_MATERIALS"/Effects/dither.mat"), "HudDither", 10, AdvDitherShaderUniforms);
    RegisterHudPostProcessShader(
        AssetGet_Material(ASSET_MATERIALS"/Effects/look_up_table.mat"), "HudLUT", 20, AdvLUTShaderUniforms);
}

void ModuleGameUpdate()
{
	UpdateWorldState();
}

void ModuleCleanup()
{
	RELEASE(player_inventory);
}