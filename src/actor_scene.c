#include "actor_scene.h"
#include "scene_entry.h"
#include "scene_library.h"
#include "actor_factory.h"
#include "raylib.h"
#include "globals.h"

// private header
SceneID next_scene;
EntranceID next_entrance;
struct Actor* current_scene = NULL;
int unload_previous_scene = TRUE;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LoadScene(SceneID id, EntranceID entrance, int unload_previous)
{
	if (unload_previous)
		UnloadScene();
	next_scene = id;
	next_entrance = entrance;
	ACTOR_FACTORY(act_scene, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
}

void ReloadScene()
{
	UnloadScene();
	ACTOR_FACTORY(act_scene, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
}

void UnloadScene()
{
	if (current_scene == NULL)
		return;
	ACTOR_DESTROY(current_scene);
	current_scene = NULL;
}

void actor_scene_init(struct Actor* scene)
{
	// Unload previous scene
	UnloadScene();
	current_scene = scene;

	// Setup scene actor with the intended functions
	SCENE_LIBRARY(scene, next_scene);

	// Load assets
	if (ACTOR_HAS(scene, func_load_preloadassets))
		scene->func_load_preloadassets(scene);

	// Check for our entrance
	struct Actor* entrance = NULL;


	// Backup entrance
	if (entrance == NULL)
	{

	}

	// Spawn player
	if (entrance != NULL && next_entrance < NO_PLAYER_SCENE)
	{

	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

