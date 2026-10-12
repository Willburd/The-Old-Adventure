#ifndef __SCENE_ACTOR_HEADER__
#define __SCENE_ACTOR_HEADER__

#include "../scene_entry.h"

// Called at the end of scene loading once all actors are loaded for the active room
void SceneFinalizeEntry(struct Actor* current_scene, SceneID next_scene, EntranceID next_entrance);
// Called at the start of a scene's room transition
void SceneRoomChangeStart(struct Actor* scene, int new_room_index, int keep_player, int instant);
// Called at the end of a scene's room transition
void SceneRoomChangeEnd(struct Actor* scene, int new_room_index, int keep_player, int instant);

#endif