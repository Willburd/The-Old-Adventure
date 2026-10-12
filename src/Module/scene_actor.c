#include "actor_library.h"
#include "actor_scene.h"

void SceneFinalizeEntry(struct Actor* current_scene, SceneID next_scene, EntranceID next_entrance)
{

}

void SceneRoomChangeStart(struct Actor* scene, int new_room_index, int keep_player, int instant)
{
	if (keep_player)
	{
		struct Actor* player = FINDACTOR_BYTYPE(act_error); // TODO - Your player actor here
		if (player)
			player->current_room_index = ACTOR_HAS_NO_ROOM_INDEX;
	}
}

void SceneRoomChangeEnd(struct Actor* scene, int new_room_index, int keep_player, int instant)
{
	if (keep_player)
	{
		struct Actor* player = FINDACTOR_BYTYPE(act_error); // TODO - Your player actor here
		if (player)
			player->current_room_index = new_room_index;
	}
}

