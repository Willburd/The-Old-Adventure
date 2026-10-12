#include "module_main.h"
#include "world_state.h"
#include "inventory.h"
#include "../assets.h"
#include "adv_postprocessing.h"
#include <raylib.h>

void ModuleInit()
{
	MALLOC_SET(Inventory, player_inventory, NULL);
	InitWorldState();
}

void ModuleLateInit()
{
    RegisterWorldPostProcessShader(
        AssetGet_Material(ASSET_MATERIALS"/Effects/dither.mat"), "WorldDither", 10, AdvDitherShaderUniforms);
    RegisterWorldPostProcessShader(
        AssetGet_Material(ASSET_MATERIALS"/Effects/look_up_table.mat"), "WorldLUT", 20, AdvLUTShaderUniforms);
    RegisterWorldPostProcessShader(
        AssetGet_Material(ASSET_MATERIALS"/Effects/dither.mat"), "HudDither", 10, AdvDitherShaderUniforms);
    RegisterHudPostProcessShader(
        AssetGet_Material(ASSET_MATERIALS"/Effects/look_up_table.mat"), "HudLUT", 20, AdvLUTShaderUniforms);
}

void ModuleInternalUpdate()
{
#ifdef _DEBUG
    if (IsKeyPressed(KEY_F7)) // Misc debug
    {
        debug_world_actors = TRUE;
    }
    DebugWorldStateInput();
#endif
}

void ModuleGameUpdate()
{
	UpdateWorldState();
}

void ModuleCleanup()
{
	RELEASE(player_inventory);
}
