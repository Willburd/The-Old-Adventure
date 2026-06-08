#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"

void scene_title_activate_room(struct Actor* scene, int room_index, int entrance);

void scene_title_init(struct Actor* scene)
{
	// Configure scene
	scene->func_activate_room = scene_title_activate_room;

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);
}

void scene_title_activate_room(struct Actor* scene, int room_index, int entrance)
{
	// Store the current active subroom of the scene
	SceneData* data = (SceneData*)scene->data;
	data->active_room = room_index;
	data->config_flags = SCENE_CONFIG_TIMEPAUSED;

	// Actor spawns

}
