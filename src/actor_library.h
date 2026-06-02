#ifndef ACTOR_LIBRARY_HEADER
#define ACTOR_LIBRARY_HEADER

typedef enum
{
	error,
	test,		// Debugging
	scene,
	player,

} ActorTypes;

// All game actors
void scene_actor_init(struct Actor* scene);
void player_actor_init(struct Actor* player);

#endif