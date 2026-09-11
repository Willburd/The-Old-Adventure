#include <stdio.h>
#include <stdlib.h>
#include "../tools.h"
#include "../actor_scene.h"
#include "../assets.h"
#include "../collision.h"

/*******************************************
		   Large Playspace Testing
********************************************/

// Assets
#define CAVE_MODEL ASSET_MODELS"/Scenes/test_cave.glb"
static const char* loaded_materials[] = {
	ASSET_MATERIALS"/Construction/rope_A.mat",		// room 0: Bridge-rope
	ASSET_MATERIALS"/Objects/wood_door_A.mat",		// room 0: wooden poles
	ASSET_MATERIALS"/Natural/stone_B.mat",			// room 0: Stone Walls
	ASSET_MATERIALS"/Construction/bridge_A.mat",	// room 0: Bridge-wood
};

// private header
SCENE_PRELOADASSETS(cavetest);
SCENE_DRAWWORLD(cavetest);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_INIT(cavetest)
{
	// Configure scene
	SCENE_REGISTER_PRELOADASSETS(cavetest);
	SCENE_REGISTER_DRAWWORLD(cavetest);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_PRELOADASSETS(cavetest)
{
	// Load model
	LoadAsset_Model(CAVE_MODEL, FALSE);
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));

	// Set collision data
	Asset* model_asset = AssetGetPackage(CAVE_MODEL);
	REGISTER_COLLISION_MESH(scene, model_asset, "Cave-Stone", COL_LAYER_WORLD | COL_LAYER_CAMERA);
	REGISTER_COLLISION_MESH(scene, model_asset, "Cave-Bridge", COL_LAYER_WORLD);
	REGISTER_COLLISION_MESH(scene, model_asset, "Cave-Wood", COL_LAYER_WORLD);
	REGISTER_COLLISION_MESH(scene, model_asset, "Cave-Rope", COL_LAYER_WORLD);
}

SCENE_DRAWWORLD(cavetest)
{
	DrawAllModelMeshes(scene, CAVE_MODEL, loaded_materials);
}