#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"

SCENE_ACTIVATE_ROOM(title);

SCENE_INIT(title)
{
	// Configure scene
	SCENE_REGISTER_ACTIVATE_ROOM(title);

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);
}

SCENE_ACTIVATE_ROOM(title)
{
	// Store the current active subroom of the scene
	SceneData* data = (SceneData*)scene->data;
	data->active_room = room_index;
	data->config_flags = SCENE_CONFIG_TIMEPAUSED;

	// Actor spawns

}
