#include <stdio.h>
#include <stdlib.h>
#include "../tools.h"
#include "../actor_factory.h"
#include "../actor_scene.h"
#include "../scene_entry.h"
#include "../assets.h"
#include "../models.h"
#include "../materials.h"
#include "../game_draw.h"
#include "actor_entrance.h"
#include "actor_trigger_exit.h"
#include "../collision.h"
#include "world_state.h"

/*******************************************
		   Large Playspace Testing
********************************************/

// Assets
#define FIELD_MODEL ASSET_MODELS"/Scenes/test_field.glb"

#define GRASS_MAT ASSET_MATERIALS"/Natural/grass_A.mat"
#define WOOD_MAT ASSET_MATERIALS"/Objects/door_wood_A.mat"
#define STONE_MAT ASSET_MATERIALS"/Natural/stone_B.mat"
#define RAILWAY_MAT ASSET_MATERIALS"/Construction/railway_A.mat"

// private header
SCENE_PRELOADASSETS(fieldtest);
SCENE_ACTIVATE_ROOM(fieldtest);
SCENE_PREPARE_ACTORS(fieldtest);
SCENE_DRAWWORLD(fieldtest);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_INIT(fieldtest)
{
	// Configure scene
	SCENE_REGISTER_PRELOADASSETS(fieldtest);
	SCENE_REGISTER_ACTIVATE_ROOM(fieldtest);
	SCENE_REGISTER_PREPARE_ACTORS(fieldtest);
	SCENE_REGISTER_DRAWWORLD(fieldtest);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_PRELOADASSETS(fieldtest)
{
	// Load model
	Asset* model_asset = LoadAsset_Model(FIELD_MODEL, FALSE);

	// Load Materials
	LoadAsset_Material(GRASS_MAT, FALSE);
	LoadAsset_Material(WOOD_MAT, FALSE);
	LoadAsset_Material(STONE_MAT, FALSE);
	LoadAsset_Material(RAILWAY_MAT, FALSE);

	// Set collision data
	REGISTER_COLLISION_MESH(scene, model_asset, "test_field-Grass", COL_LAYER_WORLD | COL_LAYER_CAMERA);
	REGISTER_COLLISION_MESH(scene, model_asset, "test_field-Bridge", COL_LAYER_WORLD | COL_LAYER_CAMERA);
	REGISTER_COLLISION_MESH(scene, model_asset, "test_field-Rockwalls", COL_LAYER_WORLD | COL_LAYER_CAMERA);
	REGISTER_COLLISION_MESH(scene, model_asset, "test_field-Railway", COL_LAYER_WORLD | COL_LAYER_CAMERA);
}

SCENE_ACTIVATE_ROOM(fieldtest)
{
	LoadCustomLayer(scene, IsDay() ? "Day" : "Night");
}

SCENE_PREPARE_ACTORS(fieldtest)
{
	struct Actor* act = FINDACTOR_BYTAG("test_tree");
	if(act)
		printf("Tree Test Actor's UUID was: %llu \n", act->uuid);
	else
		printf("Tree Test was not found! \n");
}

SCENE_DRAWWORLD(fieldtest)
{
	Asset* model_asset = AssetGetPackage(FIELD_MODEL);
	STANDARD_SHADER_DRAW(scene, model_asset, GRASS_MAT, "test_field-Grass");
	STANDARD_SHADER_DRAW(scene, model_asset, WOOD_MAT, "test_field-Bridge");
	STANDARD_SHADER_DRAW(scene, model_asset, STONE_MAT, "test_field-Rockwalls");
	STANDARD_SHADER_DRAW(scene, model_asset, RAILWAY_MAT, "test_field-Railway");
}