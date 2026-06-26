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
#define FIELD_ASSET_MAIN_MODEL ASSET_MODELS"/Scenes/test_room.glb"
#define FIELD_ASSET_MAIN_MATERIAL ASSET_MATERIALS"/Objects/wood.mat"

// Utility
#define MAIN_MODEL_MESH_MAIN 0
#define MAIN_MODEL_MESH_COLLISION 0
#define MAIN_MODEL_MATERIAL_MAIN 0

// private header
SCENE_PRELOADASSETS(fieldtest);
SCENE_CLEANUP(fieldtest);
SCENE_ACTIVATE_ROOM(fieldtest);
SCENE_LIGHTNODES(fieldtest);
SCENE_DRAWWORLD(fieldtest);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_INIT(fieldtest)
{
	// Configure scene
	SCENE_REGISTER_PRELOADASSETS(fieldtest);
	SCENE_REGISTER_CLEANUP(fieldtest);
	SCENE_REGISTER_LIGHTNODES(fieldtest);
	SCENE_REGISTER_ACTIVATE_ROOM(fieldtest);
	SCENE_REGISTER_DRAWWORLD(fieldtest);

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_PRELOADASSETS(fieldtest)
{
	// Load model
	Asset* model_asset = LoadAsset_Model(FIELD_ASSET_MAIN_MODEL, FALSE);
	LoadAsset_Material(FIELD_ASSET_MAIN_MATERIAL, FALSE);

	// Set collision data
	scene->collision_flags = COL_LAYER_WORLD;
	CollisionRegister(scene, &model_asset->mdl->meshes[MAIN_MODEL_MESH_COLLISION]);
}

SCENE_CLEANUP(fieldtest)
{
	// clear collision data
	CollisionResign(scene, &AssetGet_Model(FIELD_ASSET_MAIN_MODEL)->meshes[MAIN_MODEL_MESH_COLLISION]);
}

SCENE_ACTIVATE_ROOM(fieldtest)
{
	// Store the current active subroom of the scene
	SceneData* data = (SceneData*)scene->data;
	data->active_room = room_index;
	data->config_flags = 0;

	// Exits
	EXIT_TRIGGER_CREATE(scene_fieldtest, ent_caveA, scene, (Vector3) { 0.0f, 0.0f, 36.0f }, 18.0f);

	// Entrances
	ENTRANCE_CREATE(ent_debugentrance, scene, (Vector3) { 2.0, 0, 2 }, (Vector3) { -1, 0, 2 });
	ENTRANCE_CREATE(ent_caveA, scene, (Vector3) { -0.2f, -1.1f, 17.5f }, (Vector3) { -0.21f, -1.1f, 15.0f });

	// Actor spawns
	ACTOR_FACTORY(act_skybox, scene, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
	ACTOR_FACTORY(act_signpost, scene, (Vector3){ 2.0f, 0.0f, 0.0f }, QuaternionIdentity(), Vector3One(), Vector3Zero());
	ACTOR_FACTORY(act_fire, scene, (Vector3) { 3.0f, 2.0f, 1.0f }, QuaternionIdentity(), Vector3One(), Vector3Zero());
}

SCENE_LIGHTNODES(fieldtest)
{
	LIGHT_NODE_CAVE(-1.5f, 0.15f, 36.0f, 15.0f);
}

SCENE_DRAWWORLD(fieldtest)
{
	Material* mat = AssetGet_Material(FIELD_ASSET_MAIN_MATERIAL);
	shader_update_fog(mat->shader);
	shader_update_lights(mat->shader);

	DrawMesh(
		AssetGet_Model(FIELD_ASSET_MAIN_MODEL)->meshes[MAIN_MODEL_MESH_MAIN],
		*mat,
		GetMatrix(scene)
	);
}