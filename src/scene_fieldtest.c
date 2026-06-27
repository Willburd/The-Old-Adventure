#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"
#include "assets.h"
#include "materials.h"
#include "game_draw.h"
#include "actor_entrance.h"
#include "actor_trigger_exit.h"
#include "collision.h"
#include "world_state.h"
#include "light_tools.h"

// Assets
#define FIELD_MODEL ASSET_MODELS"/Scenes/test_room.glb"
#define FIELD_MATERIAL_MAIN ASSET_MATERIALS"/Objects/wood.mat"

// Utility
#define FIELD_MAIN 0
#define FIELD_COLLISION 0

// private header
SCENE_PRELOADASSETS(Sfieldtest);
SCENE_CLEANUP(Sfieldtest);
SCENE_ACTIVATE_ROOM(Sfieldtest);
SCENE_LIGHTNODES(Sfieldtest);
SCENE_DRAWWORLD(Sfieldtest);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_INIT(Sfieldtest)
{
	// Configure scene
	SCENE_REGISTER_PRELOADASSETS(Sfieldtest);
	SCENE_REGISTER_CLEANUP(Sfieldtest);
	SCENE_REGISTER_LIGHTNODES(Sfieldtest);
	SCENE_REGISTER_ACTIVATE_ROOM(Sfieldtest);
	SCENE_REGISTER_DRAWWORLD(Sfieldtest);

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_PRELOADASSETS(Sfieldtest)
{
	// Load model
	Asset* model_asset = LoadAsset_Model(FIELD_MODEL, FALSE);
	LoadAsset_Material(FIELD_MATERIAL_MAIN, FALSE);

	// Set collision data
	CollisionRegister(scene, &model_asset->mdl->meshes[FIELD_COLLISION], COL_LAYER_WORLD | COL_LAYER_CAMERA);
}

SCENE_CLEANUP(Sfieldtest)
{
	// clear collision data
	CollisionResign(scene, &AssetGet_Model(FIELD_MODEL)->meshes[FIELD_COLLISION]);
}

SCENE_ACTIVATE_ROOM(Sfieldtest)
{
	// Store the current active subroom of the scene
	SceneData* data = (SceneData*)scene->data;
	data->active_room = room_index;
	data->config_flags = 0;

	// Exits
	EXIT_TRIGGER_CREATE(scene_Sfieldtest, ent_caveA, scene, (Vector3) { 0.0f, 0.0f, 36.0f }, 18.0f);

	// Entrances
	ENTRANCE_CREATE(ent_debugentrance, scene, (Vector3) { 2.0, 0, 2 }, (Vector3) { -1, 0, 2 });
	ENTRANCE_CREATE(ent_caveA, scene, (Vector3) { -0.2f, -1.1f, 17.5f }, (Vector3) { -0.21f, -1.1f, 15.0f });

	// Actor spawns
	ACTOR_FACTORY(act_skybox, scene, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());

	// Spawn area torches
	ACTOR_FACTORY(act_signpost, scene, (Vector3){ 2.0f, 0.0f, 0.0f }, QuaternionIdentity(), Vector3One(), Vector3Zero());
	ACTOR_FACTORY(act_woodtorch, scene, (Vector3) { 8.3f, -1.3f, 12.42f }, QuaternionIdentity(), Vector3One(), Vector3Zero());
	ACTOR_FACTORY(act_woodtorch, scene, (Vector3) { -10.3f, -1.0f, 13.04f }, QuaternionIdentity(), Vector3One(), Vector3Zero());
	ACTOR_FACTORY(act_woodtorch, scene, (Vector3) { -6.67f, 0.16f, -14.74f }, QuaternionIdentity(), Vector3One(), Vector3Zero());
	ACTOR_FACTORY(act_woodtorch, scene, (Vector3) { 6.11f, 0.16f, -14.42f }, QuaternionIdentity(), Vector3One(), Vector3Zero());

	/*
	// Path of torches
	struct Actor* torch = ACTOR_FACTORY(act_woodtorch, scene, (Vector3) { 48.96f, -9.88f, 2.42f }, QuaternionIdentity(), Vector3One(), Vector3Zero());
	torch->blend_color = ColorToVector4(BLUE);
	torch = ACTOR_FACTORY(act_woodtorch, scene, (Vector3) { 115.01f, -16.04f, 95.63f }, QuaternionIdentity(), Vector3One(), Vector3Zero());
	torch->blend_color = ColorToVector4(RED);
	torch = ACTOR_FACTORY(act_woodtorch, scene, (Vector3) { 185.43f, -14.75f, -28.19f }, QuaternionIdentity(), Vector3One(), Vector3Zero());
	torch->blend_color = ColorToVector4(GREEN);
	torch = ACTOR_FACTORY(act_woodtorch, scene, (Vector3) { 285.25f, -9.91f, -63.23f }, QuaternionIdentity(), Vector3One(), Vector3Zero());
	torch->blend_color = ColorToVector4(YELLOW);
	*/

	// Torch test wall
	for (int i = 0; i < 45; i++)
	{
		struct Actor* torch = ACTOR_FACTORY(act_woodtorch, scene, (Vector3) { rand() % 10, 5.0f, rand() % 10}, QuaternionIdentity(), Vector3One(), Vector3Zero());
		torch->blend_color = ColorToVector4((Color){ rand() % 256, rand() % 256, rand() % 256, 255 });
	}

}

SCENE_LIGHTNODES(Sfieldtest)
{
	LIGHT_NODE_CAVE(-1.5f, 0.15f, 36.0f, 15.0f);
}

SCENE_DRAWWORLD(Sfieldtest)
{
	Material* mat = AssetGet_Material(FIELD_MATERIAL_MAIN);
	shader_update_fog(mat->shader);
	shader_update_lights(mat->shader);

	DrawMesh(
		AssetGet_Model(FIELD_MODEL)->meshes[FIELD_MAIN],
		*mat,
		GetMatrix(scene)
	);
}