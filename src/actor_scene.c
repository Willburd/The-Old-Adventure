#include "actor_scene.h"
#include "scene_entry.h"
#include "actor_factory.h"
#include "raylib.h"
#include "globals.h"

// public header
const SceneID game_start_scene = debugscene;

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
	ACTOR_FACTORY(scene, (Vector3) { 0, 0, 0 }, QuaternionIdentity(), Vector3One(), (Vector3) { 0, 0, 0 });
}

void ReloadScene()
{
	UnloadScene();
	ACTOR_FACTORY(scene, (Vector3) { 0, 0, 0 }, QuaternionIdentity(), Vector3One(), (Vector3) { 0, 0, 0 });
}

void UnloadScene()
{
	if (current_scene == NULL)
		return;
	ACTOR_DESTROY(current_scene);
	current_scene = NULL;
}

void scene_actor_init(struct Actor* scene)
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

#define MAKE_SCENE_INIT(x,y) if(scene_id == x){scene->func_init = y;scene->func_init(scene);}
inline void SCENE_LIBRARY(struct Actor* scene, SceneID scene_id)
{
	MAKE_SCENE_INIT(debugscene, scene_debugscene_init);
	MAKE_SCENE_INIT(titlescene, scene_titlescene_init);
	MAKE_SCENE_INIT(testscene, scene_testscene_init);
}