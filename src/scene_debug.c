#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_library.h"
#include "scene_entry.h"
#include "game_draw.h"
#include "camera.h"
#include "globals.h"
#include "input.h"

void scene_debug_update(struct Actor* scene);
void scene_debug_drawhud(struct Actor* scene, double tick_percent);

void scene_debug_init(struct Actor* scene)
{
	// Configure scene
	scene->func_update = scene_debug_update;
	scene->func_postdrawhud = scene_debug_drawhud;

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);

	// Debug info actor
	if(!FINDACTORTYPE(act_debug))
		ACTOR_FACTORY(act_debug, NULL, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());

	// Set sky color
	clear_background_color = BLACK;
}

#define MENUINDEX utilityA1

void scene_debug_update(struct Actor* scene)
{
	SceneData* our_data = (SceneData*)scene->data;

	if (CHECK_INPUTPRESSED(input_up))
	{
		our_data->MENUINDEX -= 1;
		if (our_data->MENUINDEX < 0) our_data->MENUINDEX = 0;
	}
	if (CHECK_INPUTPRESSED(input_down))
	{
		our_data->MENUINDEX += 1;
		if (our_data->MENUINDEX >= LAST_SCENE) our_data->MENUINDEX = LAST_SCENE-1;
	}

	if (CHECK_INPUTPRESSED(input_confirm))
	{
		LoadScene(our_data->MENUINDEX, ent_debugentrance);
		return;
	}
}

void scene_debug_drawhud(struct Actor* scene, double tick_percent)
{
	SceneData* our_data = (SceneData*)scene->data;

	DrawText("Load Scene:", 100, 20, 5, WHITE);

	for (int i = 0; i < LAST_SCENE; i++)
	{
		char* entry = scene_name(i);
		char* pretex = our_data->MENUINDEX == i ? ">" : "";
		DrawText(TextFormat("%s%s", pretex, entry), 120, 30 + (i * 8), 5, WHITE);
	}
}

