#include "actor_library.h"
#include "actor_scene.h"
// Adventure
#include "../actor_factory.h"
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
		FAST_ACTOR(act_player, current_scene);
		if (entrance != NULL) // Enter the scene from this entrance if we have one
			actor_entrance_startentry(entrance);
	}

	// Reset respawn position
	struct Actor* player = FINDACTOR_BYTYPE(act_player);
	if (ACTOR_EXISTS(player))
	{
		player->home_position = player->position;
		player->home_rotation = player->rotation;
	}

	// Start scene's music
	PlayerHighestPriorityMusic();
}

void SceneRoomChangeStart(struct Actor* scene, int new_room_index, int keep_player, int instant)
{
	if (keep_player)
	{
		struct Actor* player = FINDACTOR_BYTYPE(act_player); // Adventure
		if (player)
			player->current_room_index = ACTOR_HAS_NO_ROOM_INDEX;
	}
}

void SceneRoomChangeEnd(struct Actor* scene, int new_room_index, int keep_player, int instant)
{
	if (keep_player)
	{
		struct Actor* player = FINDACTOR_BYTYPE(act_player); // Adventure
		if (player)
			player->current_room_index = new_room_index;
	}
}
