#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"

void scene_debug_destroy(struct Actor* scene);

void scene_debug_init(struct Actor* scene)
{
	// Configure scene
	scene->func_destroy = scene_debug_destroy;
	MALLOC_ACTOR_DATA(SceneData, scene->data);

	// Actor spawns
	for (int i = 0; i < 100; i++)
	{
		ACTOR_FACTORY(act_test, (Vector3) { (float)(rand() % 300), (float)(rand() % 300), 0 }, QuaternionIdentity(), Vector3One(), (Vector3) { 1, 1, 0 });
	}
}

void scene_debug_destroy(struct Actor* scene)
{

}
