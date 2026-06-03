#include <stdlib.h>
#include <stdio.h>
#include "return_codes.h"
#include "raylib.h"
#include "raymath.h"
#include "actor.h"
#include "game_update.h"
#include "scene_entry.h"
#include "globals.h"


int current_actor_cap = 0;
int total_actors = 0;
uint64_t current_unique_id = 0;

void game_update()
{

#ifdef _DEBUG
	// Debugcode
	if (IsKeyPressed(KEY_F4))
	{
		LoadScene(scene_debug, ent_title, TRUE);
	}
#endif

	// Preupdate and state control
	int cap_actor = 0;
	int shift_index = -1;
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* update_actor = world_actors[i];
		if (update_actor == NULL)
		{
			// Store the first free slot 
			if (shift_index == -1)
				shift_index = i;
			continue;
		}
		if (shift_index > -1)
		{
			// Shift the actor back to the last free slot.
			update_actor->index = shift_index;
			world_actors[shift_index] = update_actor;
#ifdef _DEBUG
			printf("Actor slot shift: %i -> %i\n", i, shift_index);
#endif
			// Clear the current slot
			world_actors[i] = NULL;
			shift_index = i;
		}

		// Store previous state values
		update_actor->last_position.x = update_actor->position.x;
		update_actor->last_position.y = update_actor->position.y;
		update_actor->last_position.z = update_actor->position.z;

		update_actor->last_velocity.x = update_actor->velocity.x;
		update_actor->last_velocity.y = update_actor->velocity.y;
		update_actor->last_velocity.z = update_actor->velocity.z;

		update_actor->last_scale.x = update_actor->scale.x;
		update_actor->last_scale.y = update_actor->scale.y;
		update_actor->last_scale.z = update_actor->scale.z;

		update_actor->last_rotation.x = update_actor->rotation.x;
		update_actor->last_rotation.y = update_actor->rotation.y;
		update_actor->last_rotation.z = update_actor->rotation.z;
		update_actor->last_rotation.w = update_actor->rotation.w;

		// Handle actor preupdate
		if (ACTOR_HAS(update_actor, func_preupdate))
			update_actor->func_preupdate(update_actor);

		// Move actor if they have velocity
		if (Vector3Distance(Vector3Zero(), update_actor->velocity) > 0.0)
		{
			update_actor->position.x += update_actor->velocity.x;
			update_actor->position.y += update_actor->velocity.y;
			update_actor->position.z += update_actor->velocity.z;
		}

		// Update the current cap of actors
		if (update_actor->index > cap_actor)
			cap_actor = update_actor->index;
	}
	current_actor_cap = cap_actor;
	// Primary update
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor * update_actor = world_actors[i];
		if (!ACTOR_EXISTS(update_actor))
			continue;
		if (ACTOR_HAS(update_actor, func_update))
			update_actor->func_update(update_actor);
	}
	// Post update
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* update_actor = world_actors[i];
		if (!ACTOR_EXISTS(update_actor))
			continue;
		if (ACTOR_HAS(update_actor, func_postupdate))
			update_actor->func_postupdate(update_actor);
	}
}