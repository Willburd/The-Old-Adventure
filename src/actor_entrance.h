#ifndef ACTOR_ENTRANCE_HEADER
#define ACTOR_ENTRANCE_HEADER

#include "raylib.h"
#include "actor.h"

typedef struct 
{
	int entrance_id;
} EntranceData;

void actor_entrance_startentry(struct Actor* entrance);
void actor_entrance_setup(struct Actor* entrance, Vector3 startpos, Vector3 endpos);
struct Actor* entrance_create(int entrance_id, struct Actor* scene, Vector3 s_pos, Vector3 e_pos);

#endif