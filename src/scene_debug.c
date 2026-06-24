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
#include "actor_fadein.h"

SCENE_ACTIVATE_ROOM(debug);
SCENE_UPDATE(debug);
SCENE_DRAWHUD(debug);

SCENE_INIT(debug)
{
	// Configure scene
	SCENE_REGISTER_ACTIVATE_ROOM(debug);
	SCENE_REGISTER_UPDATE(debug);
	SCENE_REGISTER_DRAWHUD(debug);

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);
}

SCENE_ACTIVATE_ROOM(debug)
{
	// Store the current active subroom of the scene
	SceneData* data = (SceneData*)scene->data;
	data->active_room = room_index;
	data->config_flags = SCENE_CONFIG_TIMEPAUSED;

	// Set sky color
	clear_background_color = BLACK;

	// Debug info actor
	if (!FINDACTORTYPE(act_debug))
		ACTOR_FACTORY(act_debug, NULL, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
}

#define MENUINDEX utilityA1

SCENE_UPDATE(debug)
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
		FADEIN_CREATE(BLACK);
		TransferScene(our_data->MENUINDEX, ent_debugentrance);
		return;
	}
}

SCENE_DRAWHUD(debug)
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

