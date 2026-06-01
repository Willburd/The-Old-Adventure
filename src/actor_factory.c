#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "tools.h"
#include "return_codes.h"
#include "actor.h"
#include "actor_factory.h"
#include "raymath.h"
#include "game_update.h"
#include "actor_player.h"

int actor_compare(const void* a, const void* b, void* udata) {
	const struct Actor* ua = a;
	const struct Actor* ub = b;
	return strcmp(ua->uuid, ub->uuid);
}

uint64_t actor_hash(const void* item, uint64_t seed0, uint64_t seed1) {
	const struct Actor* actor = item;
	return hashmap_sip(actor->uuid, strlen(actor->uuid), seed0, seed1);
}

// This should not be needed under normal release params
#ifdef _DEBUG
void print_uuid(char* input)
{
	printf("[");
	for (int i = 0; i < sizeof(uint64_t); i++) {
		if(i > 0) printf(":");
		printf("%02X", (unsigned char)input[i]);
	}
	printf("]");
}
#endif

// Creates an actor in the world.
struct Actor* ACTOR_FACTORY(actor_types actor_type, Vector3 at_position, Vector3 initial_velocity)
{
	if (current_actor_cap >= ACTOR_LIMIT)
	{
		perror("No free actor slots available!\n");
		exit(ERR_NOALLOC);
	}

	// Allocate an actor as we have a free slot
	total_actors++;
	MALLOC(struct Actor, actor);

	// Setup actor
	ACTOR_CLEAR(actor);
	if (ACTOR_HAS(actor, func_load_preloadassets))
		actor->func_load_preloadassets(actor);

	// Set unique ID. Turns a uint64 into a char string for the hashtable
	current_unique_id++;
	MALLOC_SIZE(char, sizeof(uint64_t), set_uuid, 0);
	memcpy(set_uuid, &current_unique_id, sizeof(uint64_t));
	actor->uuid = set_uuid;
	hashmap_set(loaded_actors, actor);

	// Set position
	ACTOR_POS_SNAP(actor, at_position);
	ACTOR_VEL_RESET(actor, initial_velocity);
	
	// Configure to type of actor made
	ACTOR_LIBRARY(actor, actor_type);

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
		printf("Actor spawn slot: %i ", i);
		print_uuid(actor->uuid);
		printf("\n");
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
	printf("Actor slot destroy: %i ", actor->index);
	print_uuid(actor->uuid);
	printf("\n");
#endif
	total_actors--;
	if (ACTOR_HAS(actor, func_destroy))
		actor->func_destroy(actor);
	// Wipedata
	world_actors[actor->index] = NULL;
	hashmap_delete(loaded_actors, actor);
	free(actor->uuid); // malloc chararray
	if (ACTOR_HAS(actor, data))
		free(actor->data);
	ACTOR_CLEAR(actor);
	free(actor);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Actor library. Contains all actor polymorphs and where their init function pointers are.
// This only handles the actor_init function. Those functions set the rest of their pointers.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAKE_ACTOR_INIT(x,y) if(actor_type == x){actor->func_init = y; if(actor->func_init != NULL) actor->func_init(actor);}
inline void ACTOR_LIBRARY(struct Actor* actor, actor_types actor_type)
{
	MAKE_ACTOR_INIT(player, player_actor_init);
}