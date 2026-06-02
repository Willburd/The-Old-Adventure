#ifndef ACTOR_LIBRARY_HEADER
#define ACTOR_LIBRARY_HEADER

#include "actor.h"

typedef enum
{
	error,
	test,		// Debugging
	scene,
	player,
	LAST_ACTOR

} ActorTypes;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Actor library. Contains all actor polymorphs and where their init function pointers are.
// This only handles the actor_init function. Those functions set the rest of their pointers.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void actor_test_init(struct Actor* actor);
void actor_scene_init(struct Actor* scene);
void actor_player_init(struct Actor* player);

#define MAKE_ACTOR_INIT(x,y) if(actor_type == x){actor->func_init = y; if(actor->func_init != NULL) actor->func_init(actor);}
inline void ACTOR_LIBRARY(struct Actor* actor, ActorTypes actor_type)
{
	MAKE_ACTOR_INIT(test, actor_test_init);
	MAKE_ACTOR_INIT(player, actor_player_init);
	MAKE_ACTOR_INIT(scene, actor_scene_init);
}

#endif