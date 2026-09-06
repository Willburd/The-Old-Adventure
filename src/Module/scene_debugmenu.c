#include <stdio.h>
#include <stdlib.h>
#include "../tools.h"
#include "../actor_factory.h"
#include "../actor_scene.h"
#include "../Module/scene_library.h"
#include "../scene_entry.h"
#include "../game_draw.h"
#include "../camera.h"
#include "../globals.h"
#include "../input.h"

// Assets
static const char* loaded_materials[] = { NULL };

// Utility
#define MENUINDEX utilityA1

// private header
SCENE_ACTIVATE_ROOM(debugmenu);
SCENE_UPDATE(debugmenu);
SCENE_DRAWHUD(debugmenu);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_INIT(debugmenu)
{
	// Configure scene
	SCENE_REGISTER_ACTIVATE_ROOM(debugmenu);
	SCENE_REGISTER_UPDATE(debugmenu);
	SCENE_REGISTER_DRAWHUD(debugmenu);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_ACTIVATE_ROOM(debugmenu)
{
	// Debug info actor
	if (!FINDACTOR_BYTYPE(act_debug))
		ACTOR_FACTORY(NULL, act_debug, NULL, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero(), Vector3Zero());
}

SCENE_UPDATE(debugmenu)
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
		TransferScene(our_data->MENUINDEX, ent_debugentrance);
		return;
	}
}

SCENE_DRAWHUD(debugmenu)
{
	SceneData* our_data = (SceneData*)scene->data;

	DrawText("Load Scene:", 100, 20, 5, WHITE);

	for (int i = 0; i < LAST_SCENE; i++)
	{
		char* pretex = our_data->MENUINDEX == i ? ">" : "";
		DrawText(TextFormat("%s%s", pretex, all_scene_names[i]), 120, 30 + (i * 8), 5, WHITE);
	}
}

