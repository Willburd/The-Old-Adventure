#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "tools.h"
#include "return_codes.h"
#include "actor.h"
#include "actor_factory.h"
#include "game_update.h"

// Creates an actor in the world.
struct Actor* ACTOR_FACTORY(ActorTypes actor_type, struct Actor* actor_parent, Vector3 at_position, Quaternion at_rotation, Vector3 at_scale, Vector3 initial_velocity)
{
	if (current_actor_cap >= ACTOR_LIMIT)
	{
		perror("ACTOR SPAWN:No free actor slots available!\n");
		exit(ERR_NOALLOC);
	}

	// Allocate an actor as we have a free slot
	total_actors++;
	MALLOC(struct Actor, actor, NULL);

	// Setup actor
	ACTOR_CLEAR(actor);
	actor->uuid = ++current_unique_id;
	actor->actor_type = actor_type;
	actor->parent = actor_parent;

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
		printf("ACTOR SPAWN: [type: %s] slot: %i [%llu]\n", actor_name(actor->actor_type), actor->index, actor->uuid);
#endif
		return actor;
	}

	// Out of slots
	perror("ACTOR SPAWN: slot load failed!\n");
	ACTOR_DESTROY(actor);
	exit(ERR_NOALLOC);
}

// Removes an actor from the world.
void ACTOR_DESTROY(struct Actor* actor)
{
	// No recursive destroy
	if (actor->index == -1)
		return;

	// Remove all children
	ACTOR_DESTROY_CHILDREN(actor);

	// Call destroy actions
#ifdef _DEBUG
	printf("ACTOR DESTROY: [type: %02x] slot: %i [%llu]\n", actor->actor_type, actor->index, actor->uuid);
#endif
	total_actors--;
	if (ACTOR_HAS(actor, func_destroy))
		actor->func_destroy(actor);

	// Wipedata
	world_actors[actor->index] = NULL;
	if (ACTOR_HAS(actor, data))
		free(actor->data);
	ACTOR_CLEAR(actor);
	free(actor);
}

void ACTOR_DESTROY_UUID(uint64_t uuid)
{
	for (int i = 0; i < current_actor_cap; i++)
	{
		struct Actor* check_actor = world_actors[i];
		if (!ACTOR_EXISTS(check_actor))
			continue;
		if (check_actor->uuid != uuid)
			continue;
		ACTOR_DESTROY(check_actor);
	}
}

void ACTOR_DESTROY_ALL()
{
	for (int i = 0; i < current_actor_cap; i++)
	{
		struct Actor* check_actor = world_actors[i];
		if (!ACTOR_EXISTS(check_actor))
			continue;
		ACTOR_DESTROY(check_actor);
	}
}

void ACTOR_DESTROY_CHILDREN(struct Actor* parent)
{
	for (int i = 0; i < current_actor_cap; i++)
	{
		struct Actor* check_actor = world_actors[i];
		if (!ACTOR_EXISTS(check_actor))
			continue;
		if (check_actor->parent != parent)
			continue;
		ACTOR_DESTROY(check_actor);
	}
}
