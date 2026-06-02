#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"

void scene_debugscene_destroy(struct Actor* scene);

void scene_debugscene_init(struct Actor* scene)
{
	// Configure scene
	scene->func_destroy = scene_debugscene_destroy;
	MALLOC_ACTOR_DATA(SceneData, scene->data);

	// Actor spawns
	//for (int i = 0; i < 1000; i++)
	//{
	ACTOR_FACTORY(player, (Vector3) { rand() % 300, rand() % 300, 0 }, QuaternionIdentity(), Vector3One(), (Vector3) { 0, 0, 0 });
	//}
}

void scene_debugscene_destroy(struct Actor* scene)
{

}
