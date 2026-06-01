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

#define MAKE_ACTOR_INIT(x,y) if(actor_type == x){actor->func_init = y; if(actor->func_init != NULL) actor->func_init(actor);}

int actor_compare(const void* a, const void* b, void* udata) {
	const struct Actor* ua = a;
	const struct Actor* ub = b;
	return strcmp(ua->uuid, ub->uuid);
}

uint64_t actor_hash(const void* item, uint64_t seed0, uint64_t seed1) {
	const struct Actor* asset = item;
	return hashmap_sip(asset->uuid, strlen(asset->uuid), seed0, seed1);
}

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

	// Set unique ID. 
	MALLOC_SIZE(char,sizeof(uint64_t), set_uuid);
	actor->uuid = set_uuid;
	int* dref_uuid = &set_uuid;
	dref_uuid = ++current_unique_id;
	hashmap_set(loaded_actors, actor);
	
	// Setup actor
	ACTOR_CLEAR(actor);
	if (ACTOR_HAS(actor, func_load_preloadassets))
		actor->func_load_preloadassets(actor);
	ACTOR_POS_SNAP(actor, at_position);
	ACTOR_VEL_RESET(actor, initial_velocity);
	
	// Actor library
	MAKE_ACTOR_INIT(player, player_actor_init);

	// Place in update list
	for (int i = 0; i < ACTOR_LIMIT; i++)
	{
		if (world_actors[i] != NULL)
			continue;
		world_actors[i] = actor;
		actor->index = i;
		if (i > current_actor_cap)
			current_actor_cap = i;
		printf("Actor spawn slot: %i\n", i);
		return actor;
	}

	// Out of slots
	perror("Actor slot load failed!\n");
	ACTOR_DESTROY(actor);
	exit(ERR_NOALLOC);
}

void ACTOR_DESTROY(struct Actor* actor)
{
	// No recursive destroy
	if (actor->index == -1)
		return;
	printf("Actor slot destroy: %i\n", actor->index);
	total_actors--;
	if (ACTOR_HAS(actor, func_destroy))
		actor->func_destroy(actor);
	// Wipedata
	world_actors[actor->index] = NULL;
	hashmap_delete(loaded_actors, actor);
	free(actor->uuid); // chararray
	if (ACTOR_HAS(actor, data))
		free(actor->data);
	ACTOR_CLEAR(actor);
	free(actor);
}
