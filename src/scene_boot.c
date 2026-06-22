#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "globals.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"

static void scene_boot_activate_room(struct Actor* scene, int room_index, int entrance);
static void scene_boot_update(struct Actor* scene);

void scene_boot_init(struct Actor* scene)
{
	// Configure 
	scene->func_activate_room = scene_boot_activate_room;
	scene->func_update = scene_boot_update;

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);
}

static void scene_boot_activate_room(struct Actor* scene, int room_index, int entrance)
{
	// Store the current active subroom of the scene
	SceneData* data = (SceneData*)scene->data;
	data->active_room = room_index;
	data->config_flags = SCENE_CONFIG_TIMEPAUSED;
}

static void scene_boot_update(struct Actor* scene)
{
	// Go to title
	TransferScene(scene_debug, ent_title);
}
