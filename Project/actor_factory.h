#ifndef ENTITY_HEADER
#define ENTITY_HEADER

struct Actor;

typedef enum
{
	error,
	test,		// Debugging
	scene,
	player,

} actor_types;

struct Actor* ACTOR_FACTORY(actor_types actor_type, Vector3 at_position, Vector3 initial_velocity);
void ACTOR_DESTROY(struct Actor* actor);

#endif