#include "actor_scene.h"
#include "scene_entry.h"
#include "scene_library.h"
#include "actor_factory.h"
#include "raylib.h"
#include "globals.h"
#include "assets.h"

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
	Vector3 spawn_pos = Vector3Zero(); // If we have no entrance, use 0,0,0
	Quaternion spawn_rot = QuaternionIdentity();
	struct Actor* entrance = NULL;
	struct Actor* entrance_backup = NULL;


	if (entrance == NULL) // Fallback to a debugging entrance if the scene has one
		entrance = entrance_backup;

	// Spawn player
	if (next_entrance < NO_PLAYER_SCENE)
	{
		ACTOR_FACTORY(act_player, scene, spawn_pos, spawn_rot, Vector3One(), Vector3Zero());
		if (entrance != NULL) // Perform entrance actions like aligning the camera and making the player run into the scene
		{

		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

