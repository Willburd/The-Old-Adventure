#include "actor_scene.h"
#include "scene_entry.h"
#include "scene_library.h"
#include "actor_factory.h"
#include "raylib.h"
#include "globals.h"
#include "assets.h"
#include "actor_entrance.h"
#include "tools.h"

// private header
SceneID next_scene;
EntranceID next_entrance;
struct Actor* current_scene = NULL;
int unload_previous_scene = TRUE;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LoadScene(SceneID id, EntranceID entrance)
{
	next_scene = id;
	next_entrance = entrance;
	ACTOR_FACTORY(act_scene, NULL, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
}

void ReloadScene()
{
	UnloadScene(FALSE);
	ACTOR_FACTORY(act_scene, NULL, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
}

void UnloadScene(int clear_assets)
{
	if (current_scene == NULL)
		return;
	ACTOR_DESTROY(current_scene);
	if(clear_assets)
		UnloadAllAssets(FALSE);
	current_scene = NULL;
}

void actor_scene_init(struct Actor* scene)
{
	// Unload previous scene
	UnloadScene(TRUE);
	current_scene = scene;

	// Setup scene actor with the intended functions
	SCENE_LIBRARY(scene, next_scene);

	// Load assets
	if (ACTOR_HAS(scene, func_preloadassets))
		scene->func_preloadassets(scene);

	// Activate the first room inside the scene
	if (ACTOR_HAS(scene, func_activate_room))
		scene->func_activate_room(scene, 0, next_entrance);

	// Check for entrance actors, find the one we're using.
	const struct Actor* found_group[LAST_ENTRANCE] = { 0 };
	FINDALLACTORTYPE(found_group, LAST_ENTRANCE, act_entrance);
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
	if (next_entrance < NO_PLAYER_SCENE)
	{
		if(FINDACTORTYPE(act_player) == NULL) // Only a single player
			ACTOR_FACTORY(act_player, scene, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
		if (entrance != NULL) // Enter the scene from this entrance if we have one
			actor_entrance_startentry(entrance);

		
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

