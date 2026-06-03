#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"

void scene_test_init(struct Actor* scene)
{
	// Configure scene

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);

	// Actor spawns
	for (int i = 0; i < 60; i++)
	{
		ACTOR_FACTORY(act_test, scene, (Vector3) { (float)(rand() % 300), (float)(rand() % 300), 0 }, QuaternionIdentity(), Vector3One(), (Vector3) { 1, 1, 0 });
	}

	// Function testing
	int child_count = CHILDCOUNT(scene);
	printf("children: %i\n", child_count);

	struct Actor* child_array[10] = { NULL };
	FINDACTORCHILDREN(child_array, 10, scene);
	printf("cuid: %llu\n", child_array[0]->uuid);
}
