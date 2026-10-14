#include "actor_library.h"
#include "module_main.h"
#include "world_state.h"
#include "inventory.h"
#include "../assets.h"
#include "../actor_factory.h"
#include "actor_camera.h"
#include "adv_postprocessing.h"
#include <raylib.h>

const char* ModuleGameName()
{
    return "The Old Adventures - Classic Adventure"; // Adventure
}

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

	// Time shifting
	if (IsKeyDown(KEY_EQUAL))
		daynight_cycle += 0.001f;
	if (IsKeyDown(KEY_MINUS))
	{
		daynight_cycle -= 0.001f;
		while (daynight_cycle < 0)
			daynight_cycle += 1.0f;
	}

	// Room loading
	if (debug_world_actors)
	{
		int modifier = 0;
		struct Actor* current_scene = GetCurrentScene();
		if (IsKeyDown(KEY_LEFT_CONTROL))
			modifier = 30;
		else if (IsKeyDown(KEY_LEFT_ALT))
			modifier = 20;
		else if (IsKeyDown(KEY_LEFT_SHIFT))
			modifier = 10;
		if (IsKeyPressed(KEY_ONE))
			ChangeSceneRoom(current_scene, modifier + 1, FALSE, FALSE);
		else if (IsKeyPressed(KEY_TWO))
			ChangeSceneRoom(current_scene, modifier + 2, FALSE, FALSE);
		else if (IsKeyPressed(KEY_THREE))
			ChangeSceneRoom(current_scene, modifier + 3, FALSE, FALSE);
		else if (IsKeyPressed(KEY_FOUR))
			ChangeSceneRoom(current_scene, modifier + 4, FALSE, FALSE);
		else if (IsKeyPressed(KEY_FIVE))
			ChangeSceneRoom(current_scene, modifier + 5, FALSE, FALSE);
		else if (IsKeyPressed(KEY_SIX))
			ChangeSceneRoom(current_scene, modifier + 6, FALSE, FALSE);
		else if (IsKeyPressed(KEY_SEVEN))
			ChangeSceneRoom(current_scene, modifier + 7, FALSE, FALSE);
		else if (IsKeyPressed(KEY_EIGHT))
			ChangeSceneRoom(current_scene, modifier + 8, FALSE, FALSE);
		else if (IsKeyPressed(KEY_NINE))
			ChangeSceneRoom(current_scene, modifier + 9, FALSE, FALSE);
		else if (IsKeyPressed(KEY_ZERO))
			ChangeSceneRoom(current_scene, modifier + 0, FALSE, FALSE);

		// Force return to player
		if (IsKeyPressed(KEY_P))
		{
			debug_world_actors = FALSE;
			struct Actor* cam = FINDACTOR_BYTYPE(act_camera);
			CameraData* cam_data = (CameraData*)cam->data;
			cam_data->camera_mode = CAMERA_MODE_FOLLOW;

			struct Actor* player = FINDACTOR_BYTYPE(act_player);
			if (!ACTOR_EXISTS(player))
				player = FAST_ACTOR(act_player, current_scene);
			ACTOR_POS_SNAP(player, cam->position);
		}
	}
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

void ModuleExtendShaderDefaultUniforms(Shader shader)
{
	float animation_ticker = (float)world_tick_counter;
	int loc = GetShaderLocation(shader, "uAnimator");
	SetShaderValue(shader, loc, &animation_ticker, SHADER_UNIFORM_FLOAT);
}

void ModuleExtendShaderFogUniforms(Shader shader)
{

}

void ModuleExtendShaderLightUniforms(Shader shader)
{

}
