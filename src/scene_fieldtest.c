#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"

void scene_fieldtest_preloadassets(struct Actor* scene);
void scene_fieldtest_destroy(struct Actor* scene);
void scene_fieldtest_drawworld(struct Actor* scene, double tick_percent);

void scene_fieldtest_init(struct Actor* scene)
{
	// Configure scene
	scene->func_preloadassets = scene_fieldtest_preloadassets;
	scene->func_drawworld = scene_fieldtest_drawworld;
	scene->func_destroy = scene_fieldtest_destroy;

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);

	// Actor spawns
}

void scene_fieldtest_preloadassets(struct Actor* scene)
{
}


void scene_fieldtest_drawworld(struct Actor* scene, double tick_percent)
{
	DrawCube(Vector3Zero(), 0.2f, 0.2f, 0.2f, RED);
}

void scene_fieldtest_destroy(struct Actor* scene)
{

}
