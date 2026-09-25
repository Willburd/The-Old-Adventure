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

#define ROPE_MAT ASSET_MATERIALS"/Construction/rope_A.mat"
#define WOOD_MAT ASSET_MATERIALS"/Objects/door_wood_A.mat"
#define STONE_MAT ASSET_MATERIALS"/Natural/stone_B.mat"
#define BRIDGE_MAT ASSET_MATERIALS"/Construction/bridge_A.mat"

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
	Asset* model_asset = LoadAsset_Model(CAVE_MODEL, FALSE);

	// Load Materials
	LoadAsset_Material(ROPE_MAT, FALSE);
	LoadAsset_Material(WOOD_MAT, FALSE);
	LoadAsset_Material(STONE_MAT, FALSE);
	LoadAsset_Material(BRIDGE_MAT, FALSE);

	// Set collision data
	REGISTER_COLLISION_MESH(scene, model_asset, "Cave-Stone", COL_LAYER_WORLD | COL_LAYER_CAMERA);
	REGISTER_COLLISION_MESH(scene, model_asset, "Cave-Bridge", COL_LAYER_WORLD);
	REGISTER_COLLISION_MESH(scene, model_asset, "Cave-Wood", COL_LAYER_WORLD);
	REGISTER_COLLISION_MESH(scene, model_asset, "Cave-Rope", COL_LAYER_WORLD);
}

SCENE_DRAWWORLD(cavetest)
{
	Asset* model_asset = AssetGetPackage(CAVE_MODEL);
	STANDARD_SHADER_DRAW(scene, model_asset, STONE_MAT, "Cave-Stone");
	STANDARD_SHADER_DRAW(scene, model_asset, BRIDGE_MAT, "Cave-Bridge");
	STANDARD_SHADER_DRAW(scene, model_asset, WOOD_MAT, "Cave-Wood");
	STANDARD_SHADER_DRAW(scene, model_asset, ROPE_MAT, "Cave-Rope");
}