#include "actor_scene.h"
#include "scene_entry.h"
#include "scene_library.h"
#include "actor_factory.h"
#include "raylib.h"
#include "globals.h"
#include "assets.h"
#include "tools.h"
#include "game_draw.h"
// Adventure
#include "actor_entrance.h"

// private header
static SceneID next_scene;
static int next_room = -1;
static EntranceID next_entrance;
static struct Actor* current_scene = NULL;
static int unload_previous_scene = TRUE;
static void LoadRoomLayer(struct Actor* scene, char* layer_path);
static void FinalizeRoomChange();

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

	// Set default scene data.
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);

	// Load assets. This has polymorphed to the SCENE'S assets since the initial call to preloadassets in actor library.
	if (ACTOR_HAS(scene, func_preloadassets))
		scene->func_preloadassets(scene);
	ChangeSceneRoom(scene, 0, FALSE, TRUE); // Activate the first room inside the scene.

	// Adventure edit begin - Check for entrance actors, find the one we're using.
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
	// Adventure edit end

	// Finalize actors
	if (ACTOR_HAS(scene, func_prepare_actors))
		scene->func_prepare_actors(scene, scene->current_room_index, next_entrance);

	printf("\n..............................................................................\n");
	printf("CHANGE FINISHED ==> %s \n", scene->actor_type_name);
	printf("********************************************************************************\n");
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
	if (next_scene >= 0)
	{
		ACTOR_FACTORY(NULL, act_scene, NULL, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
		next_scene = -1;
	}
	FinalizeRoomChange();
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
	LoadRoomLayer(scene, TextFormat("%s/%s/default.json", ASSET_SCENE, scene->actor_type_name));
	LoadRoomLayer(scene, TextFormat("%s/%s/room_%i.json", ASSET_SCENE, scene->actor_type_name, scene->current_room_index));
}

// Applies a custom layer json
void LoadCustomLayer(struct Actor* scene, char* custom_layer)
{
	SceneData* scene_data = (SceneData*)scene->data;
	LoadRoomLayer(scene, TextFormat("%s/%s/%s.json", ASSET_SCENE, scene->actor_type_name, custom_layer));
}

// Loads the specified room index of the current scene, while deleting all actors outside of it. Resets the current room if used to go to the same index as current. Instant argument should only be used when loading a newly created scene actor, and not during room changes.
void ChangeSceneRoom(struct Actor* scene, int new_room_index, int keep_player, int instant)
{
	// If we're retaining the player we need to do some magic.
	struct Actor* player = NULL;
	if (keep_player)
	{
		struct Actor* player = FINDACTORTYPE(act_player); // Adventure edit - Use our player actor
		if (player)
			player->current_room_index = ACTOR_HAS_NO_ROOM_INDEX;
	}
	// Clear puzzle flag
	SceneData* scene_data = (SceneData*)scene->data;
	scene_data->puzzle_flags = 0; // Clear puzzle flags on room change
	// Remove prior actors
	if (ACTOR_HAS(scene, func_deactivate_room))
		scene->func_deactivate_room(scene, scene->current_room_index);
	ACTOR_DESTROY_IN_ROOM(current_scene->current_room_index);
	printf(",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\n");
	printf("CHANGED ROOM: %i -> %i \n", scene->current_room_index, new_room_index);
	printf("'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''\n");
	next_room = new_room_index;
	// Activate the first room inside the scene.
	if (player)
		player->current_room_index = current_scene->current_room_index;
	if (instant)
		FinalizeRoomChange();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Apply a json layer file to the room
static void LoadRoomLayer(struct Actor* scene, char* layer_path)
{
	cJSON* json_data = ParseJsonFile(layer_path);
	if (json_data == NULL)
		return;

	// Get data config
	SceneData* data = (SceneData*)scene->data;
	data->config_flags |= CHECK_JSON_BOOL(json_data, "time_paused") ? SCENE_CONFIG_TIMEPAUSED : 0; // range: [0 - 1]
	data->config_flags |= CHECK_JSON_BOOL(json_data, "is_hot") ? SCENE_CONFIG_HOTROOM : 0; // range: [0 - 1]
	data->config_flags |= CHECK_JSON_BOOL(json_data, "is_cold") ? SCENE_CONFIG_COLDROOM : 0; // range: [0 - 1]
	data->config_flags |= CHECK_JSON_BOOL(json_data, "is_raining") ? SCENE_CONFIG_ISRAINING : 0; // range: [0 - 1]

	// Set sky color. range: [0 - 1]
	if (cJSON_IsArray(cJSON_GetObjectItem(json_data, "sky_color")))
	{
		cJSON* array = cJSON_GetObjectItem(json_data, "sky_color");
		Vector4 solved_color = {
			(float)cJSON_GetArrayItem(array, 0)->valuedouble,
			(float)cJSON_GetArrayItem(array, 1)->valuedouble,
			(float)cJSON_GetArrayItem(array, 2)->valuedouble,
			(float)cJSON_GetArrayItem(array, 3)->valuedouble
		};
		clear_background_color = Vector4ToColor(solved_color);
	}

	// Create actors from actors array in json. range: [{Object array}]
	cJSON* actor_array = cJSON_GetObjectItem(json_data, "actors");
	for (int i = 0; i < cJSON_GetArraySize(actor_array); i++)
		JSON_ACTOR_FACTORY(cJSON_GetArrayItem(actor_array, i), scene);

	// Cleanup
	cJSON_Delete(json_data);
}

// Actors require a gap between loading and unloading entire scenes. This is called from the game update before pre-update. At the same time when scene-loading is handled. Can also be called instantly from ChangeSceneRoom() on a scene's first load.
static void FinalizeRoomChange()
{
	if (next_room < 0)
		return;
	current_scene->current_room_index = next_room;
	next_room = -1;
	if (ACTOR_HAS(current_scene, func_activate_room))
		current_scene->func_activate_room(current_scene, current_scene->current_room_index, next_entrance);
	LoadSceneJSONActors(current_scene);
}