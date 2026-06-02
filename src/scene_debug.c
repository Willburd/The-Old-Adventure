#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"

void scene_debug_destroy(struct Actor* scene);
void scene_debug_update(struct Actor* scene);
void scene_debug_drawhud(struct Actor* scene, double tick_percent);

void scene_debug_init(struct Actor* scene)
{
	// Configure scene
	scene->func_destroy = scene_debug_destroy;
	scene->func_update = scene_debug_update;
	scene->func_drawhud = scene_debug_drawhud;
	MALLOC_ACTOR_DATA(SceneData, scene->data);

	// Choose a scene to warp to list
}

void scene_debug_update(struct Actor* scene)
{

}

void scene_debug_drawhud(struct Actor* scene, double tick_percent)
{

}

void scene_debug_destroy(struct Actor* scene)
{

}
