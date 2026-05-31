#ifndef ACTOR_HEADER
#define ACTOR_HEADER

#include "raylib.h"

struct Actor;
struct Actor {
	int index;

	Vector3 position;
	Vector3 velocity;
	Vector3 last_position;
	Vector3 last_velocity;

	void (*func_init)(struct Actor* player);
	void (*func_load_assets)(struct Actor* player);
	void (*func_preupdate)(struct Actor* player);
	void (*func_update)(struct Actor* player);
	void (*func_postupdate)(struct Actor* player);
	void (*func_drawworld)(struct Actor* player, float delta_time);
	void (*func_drawhud)(struct Actor* player, float delta_time);
	void (*func_destroy)(struct Actor* player);
	void* data;
};
#define ACTOR_CLEAR(x) x->index = -1;x->func_init = NULL;x->func_load_assets = NULL;x->func_destroy = NULL;x->func_preupdate = NULL;x->func_update = NULL;x->func_postupdate = NULL;x->func_drawworld = NULL;x->func_drawhud = NULL;x->data = NULL;
#define ACTOR_POS_SNAP(x, pos) x->position = pos;x->last_position = pos;
#define ACTOR_VEL_RESET(x, vel) x->velocity = vel;x->last_velocity = vel;
#define ACTOR_EXISTS(x) (x != NULL && x->index > -1)
#define ACTOR_HAS(x, func) (x->func != NULL)

#define ACTOR_LIMIT 2048

int current_actor_cap;
int total_actors;
struct Actor* world_actors[ACTOR_LIMIT];

#endif