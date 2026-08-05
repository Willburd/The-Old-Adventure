#ifndef __ACTOR_TRIGGER_EXIT_HEADER__
#define __ACTOR_TRIGGER_EXIT_HEADER__

#include "raylib.h"
#include "actor.h"

typedef struct
{
	int dest_scene;
	int dest_entrance;
	float radius;
} TriggerExitData;

struct Actor* EXIT_TRIGGER_CREATE(int destination_scene, int destination_entrance, struct Actor* scene, Vector3 pos, float radius);

#endif