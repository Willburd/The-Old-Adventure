#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"
#include "collision.h"
#include "game_draw.h"

/*******************************************
			Gameplay Debuging Room
********************************************/

// Assets
#define TESTROOM_MODEL ASSET_MODELS"/Scenes/test_room.glb"
#define TESTROOM_MATERIAL_STONE ASSET_MATERIALS"/Natural/stone_B.mat"

// Meshes
#define TESTROOM_MESH_BASE "test_room"

// private header
SCENE_PRELOADASSETS(test);
SCENE_ACTIVATE_ROOM(test);
SCENE_DRAWWORLD(test);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_INIT(test)
{
	SCENE_REGISTER_PRELOADASSETS(test);
	SCENE_REGISTER_ACTIVATE_ROOM(test);
	SCENE_REGISTER_DRAWWORLD(test);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_PRELOADASSETS(test)
{
	// Load model
	Asset* model_asset = LoadAsset_Model(TESTROOM_MODEL, FALSE);
	LoadAsset_Material(TESTROOM_MATERIAL_STONE, FALSE);

	// Set collision data
	REGISTER_COLLISION_MESH(scene, model_asset, TESTROOM_MESH_BASE"-Main", COL_LAYER_WORLD | COL_LAYER_CAMERA);
}

SCENE_ACTIVATE_ROOM(test)
{
}

SCENE_DRAWWORLD(test)
{
	Asset* model_asset = AssetGetPackage(TESTROOM_MODEL);
	STANDARD_SHADER_MATERIAL(base_mat, TESTROOM_MATERIAL_STONE, scene);

	ToaDrawMesh(
		model_asset,
		GetMeshIndex(model_asset->mesh_data, TESTROOM_MESH_BASE"-Main"),
		*base_mat,
		GetMatrix(scene),
		FALSE
	);
}