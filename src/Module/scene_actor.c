#include "../scene_entry.h"
#include "actor_library.h"
// Adventure
#include "actor_entrance.h"
#include "world_state.h"
#include "actor_musicplayer.h"

void SceneFinalizeEntry(struct Actor* current_scene, SceneID next_scene, EntranceID next_entrance)
{
	// Check for entrance actors, find the one we're using.
	struct Actor* found_group[LAST_ENTRANCE] = { 0 };
	FINDACTORGROUP_BYTYPE(found_group, LAST_ENTRANCE, act_entrance);
	struct Actor* entrance = NULL;
	struct Actor* entrance_backup = NULL;
	for (int i = 0; i < LAST_ENTRANCE; i++)
	{
		struct Actor* check_entrance = found_group[i];
		if (!ACTOR_EXISTS(check_entrance))
			continue;
		// Check if the entrance type is valid for use
		EntranceData* data = check_entrance->data;
		if (data->entrance_id == next_entrance)
			entrance = check_entrance;
		if (data->entrance_id == ent_debugentrance)
			entrance_backup = check_entrance;
	}

	// Spawn player
	ACTOR_DESTROY_TYPE(act_player); // Only a single player
	if (next_entrance < NO_PLAYER_SCENE || debug_world_actors)
	{
		ACTOR_FACTORY(NULL, act_player, current_scene, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero(), Vector3Zero());
		if (entrance != NULL) // Enter the scene from this entrance if we have one
			actor_entrance_startentry(entrance);
	}

	// Start scene's music
	PlayerHighestPriorityMusic();
}
