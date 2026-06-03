#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"

void scene_fieldtest_destroy(struct Actor* scene);

void scene_fieldtest_init(struct Actor* scene)
{
	// Configure scene
	scene->func_destroy = scene_fieldtest_destroy;

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);

	// Actor spawns
}

void scene_fieldtest_destroy(struct Actor* scene)
{

}
