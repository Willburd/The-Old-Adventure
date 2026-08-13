#include <stdlib.h>
#include <stdio.h>
#include "return_codes.h"
#include "raylib.h"
#include "raymath.h"
#include "actor_factory.h"
#include "game_update.h"
#include "collision.h"
#include "game_draw.h"
#include "scene_entry.h"
#include "globals.h"
#include "input.h"
#include "game_state.h"
// Adventure
#include "world_state.h"


int current_actor_cap = 0;
int total_actors = 0;
uint64_t current_unique_id = 0;

void game_update()
{
	// Update controller
	UpdateInputState();

	// Update window
	screenWidth = GetScreenWidth();
	screenHeight = GetScreenHeight();

	////////////////////////////////////////////////////////////////////////
	// Prepare world and renderer
	////////////////////////////////////////////////////////////////////////

	debug_current_rays = 0; // reset debug info
	light_count = 0;
	UpdateWorldState();	// Adventure edit begin - Update our worldstate
	HandleLoadNextScene();

	////////////////////////////////////////////////////////////////////////
	// Preupdate and state control
	////////////////////////////////////////////////////////////////////////

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

		// Update the current cap of actors
		if (update_actor->index > cap_actor)
			cap_actor = update_actor->index;

		// Handle actor preupdate
		if (!((update_actor->actor_flags & GAMESTATE_FILTER_MODES) & gameplay_state)) // if can update in this state
			continue;
		if (ACTOR_HAS(update_actor, func_preupdate))
			update_actor->func_preupdate(update_actor);
		if (Vector3Distance(Vector3Zero(), update_actor->velocity) > 0.0) // Move actor if they have velocity
		{
			update_actor->position.x += update_actor->velocity.x;
			update_actor->position.y += update_actor->velocity.y;
			update_actor->position.z += update_actor->velocity.z;
		}
	}
	current_actor_cap = cap_actor;

	////////////////////////////////////////////////////////////////////////
	// Primary update
	////////////////////////////////////////////////////////////////////////

	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor * update_actor = world_actors[i];
		if (!ACTOR_EXISTS(update_actor))
			continue;
		if (!((update_actor->actor_flags & GAMESTATE_FILTER_MODES) & gameplay_state)) // if can update in this state
			continue;
		if (ACTOR_HAS(update_actor, func_update))
			update_actor->func_update(update_actor);
	}


	////////////////////////////////////////////////////////////////////////
	// Post update
	////////////////////////////////////////////////////////////////////////

	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* update_actor = world_actors[i];
		if (!ACTOR_EXISTS(update_actor))
			continue;
		if (!((update_actor->actor_flags & GAMESTATE_FILTER_MODES) & gameplay_state)) // if can update in this state
			continue;
		if (ACTOR_HAS(update_actor, func_postupdate))
			update_actor->func_postupdate(update_actor);
		if (update_actor->actor_flags & ACTOR_FLAG_HAS_ANIMATIONS)
			UpdateAnimLayers(update_actor);
	}

	////////////////////////////////////////////////////////////////////////
	// Finalize before draw
	////////////////////////////////////////////////////////////////////////

	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* finalize_actor = world_actors[i];
		if (finalize_actor == NULL)
			continue;
		// Fully delete actors that no longer exist
		if (finalize_actor->is_destroying)
		{
			HandleActorFinalCleanup(finalize_actor);
			continue;
		}
		// Light update
		if (!(finalize_actor->actor_flags & ACTOR_FLAG_DISABLE_LIGHTS) && ACTOR_HAS(finalize_actor, func_append_lights))
			finalize_actor->func_append_lights(finalize_actor);
	}
}