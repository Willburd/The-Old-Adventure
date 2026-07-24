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

ACTOR_INIT(scene)
{
	// Use this for clarity
	struct Actor* scene = actor; 

	// Unload previous scene
	current_scene = scene;

	// Setup scene actor with the intended functions
	SCENE_LIBRARY(scene, next_scene);

	// Load assets. This has polymorphed to the SCENE'S assets since the initial call to preloadassets in actor library.
	if (ACTOR_HAS(scene, func_preloadassets))
		scene->func_preloadassets(scene);

	// Activate the first room inside the scene.
	if (ACTOR_HAS(scene, func_activate_room))
		scene->func_activate_room(scene, 0, next_entrance);
	LoadSceneJSONActors(scene);

	// Check for entrance actors, find the one we're using.
	struct Actor* found_group[LAST_ENTRANCE] = { 0 };
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
	ACTOR_DESTROY_TYPE(act_player); // Only a single player
	if (next_entrance < NO_PLAYER_SCENE)
	{
		ACTOR_FACTORY(NULL, act_player, current_scene, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
		if (entrance != NULL) // Enter the scene from this entrance if we have one
			actor_entrance_startentry(entrance);
	}
}

// Sets the next scene to be loaded. Will be actually loaded before the pre-update loop next gametick. DOES NOT LOAD THE SCENE ITSELF. Happens next tick, this is safe to call with ACTOR_DESTORY actions happening.
void LoadScene(SceneID id, EntranceID entrance)
{
	next_scene = id;
	next_entrance = entrance;
}

// Sets the next scene to be loaded while unloading the previous scene. See LoadScene() for details.
void TransferScene(SceneID id, EntranceID entrance)
{
	UnloadScene(TRUE);
	LoadScene(id, entrance);
}

struct Actor* GetCurrentScene()
{
	return current_scene;
}

// Loads the next scene. Called before the preupdate loop in gametick to avoid being mangled by unloading assets.
void HandleLoadNextScene()
{
	if (next_scene < 0)
		return;
	ACTOR_FACTORY(NULL, act_scene, NULL, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
	next_scene = -1;
}

// Destroys all actors that are children of the current scene, and the scene itself. Optionally unloading all non-core loaded assets with it.
void UnloadScene(int clear_assets)
{
	if (current_scene == NULL)
		return;
	ACTOR_DESTROY(current_scene);
	if (clear_assets)
		UnloadAllAssets(FALSE);
	current_scene = NULL;
}

// Loads static scene json to place actors
void LoadSceneJSONActors(struct Actor* scene)
{
	char* scene_name = scene->actor_type_name;
	SceneData* scene_data = (SceneData*)scene->data;
	int room_index = scene_data->active_room;
	// Load data if there is any
	cJSON* json_data = ParseJsonFile(TextFormat("%s/%s/default.json", ASSET_SCENE, scene_name));
	if (json_data == NULL)
		return;
	// Create actors from actors array in json
	cJSON* actor_array = cJSON_GetObjectItem(json_data, "actors");
	for (int i = 0; i < cJSON_GetArraySize(actor_array); i++)
		JSON_ACTOR_FACTORY(cJSON_GetArrayItem(actor_array, i), scene);
	// Cleanup
	cJSON_Delete(json_data);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
