#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"

void scene_title_destroy(struct Actor* scene);

void scene_title_init(struct Actor* scene)
{
	// Configure scene
	scene->func_destroy = scene_title_destroy;
	MALLOC_ACTOR_DATA(SceneData, scene->data);

	// Actor spawns
}

void scene_title_destroy(struct Actor* scene)
{

}
