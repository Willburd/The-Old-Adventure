#ifndef __ACTOR_ENTRANCE_HEADER__
#define __ACTOR_ENTRANCE_HEADER__

#include "raylib.h"
#include "actor.h"

typedef struct 
{
	int entrance_id;
} EntranceData;

void actor_entrance_startentry(struct Actor* entrance);
struct Actor* ENTRANCE_CREATE(int entrance_id, struct Actor* scene, Vector3 s_pos, Vector3 e_pos);

#endif