#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "globals.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"

SCENE_ACTIVATE_ROOM(boot);
SCENE_UPDATE(boot);

SCENE_INIT(boot)
{
	// Configure 
	SCENE_REGISTER_ACTIVATE_ROOM(boot);
	SCENE_REGISTER_UPDATE(boot);

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);
}

SCENE_ACTIVATE_ROOM(boot)
{
	// Store the current active subroom of the scene
	SceneData* data = (SceneData*)scene->data;
	data->active_room = room_index;
	data->config_flags = SCENE_CONFIG_TIMEPAUSED;
}

SCENE_UPDATE(boot)
{
	// Go to title
	TransferScene(scene_debug, ent_title);
}
