#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "tools.h"
#include "return_codes.h"
#include "actor_factory.h"
#include "game_update.h"

// Creates an actor in the world.
struct Actor* ACTOR_FACTORY(actor_types actor_type, Vector3 at_position, Quaternion at_rotation, Vector3 at_scale, Vector3 initial_velocity)
{
	if (current_actor_cap >= ACTOR_LIMIT)
	{
		perror("No free actor slots available!\n");
		exit(ERR_NOALLOC);
	}

	// Allocate an actor as we have a free slot
	total_actors++;
	MALLOC(struct Actor, actor, NULL);

	// Setup actor
	ACTOR_CLEAR(actor);
	actor->uuid = ++current_unique_id;

	// Set position
	ACTOR_POS_SNAP(actor, at_position);
	ACTOR_ROT_SNAP(actor, at_rotation);
	ACTOR_SCALE_SNAP(actor, at_scale);
	ACTOR_VEL_RESET(actor, initial_velocity);
	
	// Configure to type of actor made
	ACTOR_LIBRARY(actor, actor_type);
	if (ACTOR_HAS(actor, func_load_preloadassets))
		actor->func_load_preloadassets(actor);

	// Place in update list
	for (int i = 0; i < ACTOR_LIMIT; i++)
	{
		if (world_actors[i] != NULL)
			continue;
		world_actors[i] = actor;
		actor->index = i;
		if (i > current_actor_cap)
			current_actor_cap = i;
#ifdef _DEBUG
		printf("Actor spawn slot: %i [%llu]\n", actor->index, actor->uuid);
#endif
		return actor;
	}

	// Out of slots
	perror("Actor slot load failed!\n");
	ACTOR_DESTROY(actor);
	exit(ERR_NOALLOC);
}

// Removes an actor from the world.
void ACTOR_DESTROY(struct Actor* actor)
{
	// No recursive destroy
	if (actor->index == -1)
		return;
#ifdef _DEBUG
	printf("Actor slot destroy: %i [%llu]\n", actor->index, actor->uuid);
#endif
	total_actors--;
	if (ACTOR_HAS(actor, func_destroy))
		actor->func_destroy(actor);
	actor->uuid = 0;
	// Wipedata
	world_actors[actor->index] = NULL;
	if (ACTOR_HAS(actor, data))
		free(actor->data);
	ACTOR_CLEAR(actor);
	free(actor);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Actor library. Contains all actor polymorphs and where their init function pointers are.
// This only handles the actor_init function. Those functions set the rest of their pointers.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "actor_player.h"
#include "actor_scene.h"

#define MAKE_ACTOR_INIT(x,y) if(actor_type == x){actor->func_init = y; if(actor->func_init != NULL) actor->func_init(actor);}
inline void ACTOR_LIBRARY(struct Actor* actor, actor_types actor_type)
{
	MAKE_ACTOR_INIT(player, player_actor_init);
	MAKE_ACTOR_INIT(scene, scene_actor_init);
}