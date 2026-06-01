#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include <stdint.h>

struct Actor;

typedef enum
{
	error,
	test,		// Debugging
	scene,
	player,

} actor_types;

int actor_compare(const void* a, const void* b, void* udata);
uint64_t actor_hash(const void* item, uint64_t seed0, uint64_t seed1);

struct hashmap* loaded_actors;

struct Actor* ACTOR_FACTORY(actor_types actor_type, Vector3 at_position, Vector3 initial_velocity);
inline void ACTOR_LIBRARY(struct Actor* actor, actor_types actor_type);
void ACTOR_DESTROY(struct Actor* actor);

#endif