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
	    Coastline asthetics test room
********************************************/

// Assets
#define FIELD_MODEL ASSET_MODELS"/Scenes/lakeside_coast.glb"

#define GRASS_MAT ASSET_MATERIALS"/Natural/grass_A.mat"
#define STONE_MAT ASSET_MATERIALS"/Natural/stone_C.mat"
#define SAND_MAT ASSET_MATERIALS"/Natural/sand_A.mat"
#define GRASS_EDGE_MAT ASSET_MATERIALS"/Natural/grass_edge_A.mat"
#define PATH_MAT ASSET_MATERIALS"/Natural/path_A.mat"
#define DISTANTTREE_MAT ASSET_MATERIALS"/Trees/distant_A.mat"
#define GRASSBORDER_MAT ASSET_MATERIALS"/Natural/grass_border_A.mat"

// private header
SCENE_PRELOADASSETS(lakeside_coast);
SCENE_ACTIVATE_ROOM(lakeside_coast);
SCENE_PREPARE_ACTORS(lakeside_coast);
SCENE_DRAWWORLD(lakeside_coast);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_INIT(lakeside_coast)
{
	// Configure scene
	SCENE_REGISTER_PRELOADASSETS(lakeside_coast);
	SCENE_REGISTER_ACTIVATE_ROOM(lakeside_coast);
	SCENE_REGISTER_PREPARE_ACTORS(lakeside_coast);
	SCENE_REGISTER_DRAWWORLD(lakeside_coast);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_PRELOADASSETS(lakeside_coast)
{
	// Load model
	Asset* model_asset = LoadAsset_Model(FIELD_MODEL, FALSE);
	
	// Load Materials
	LoadAsset_Material(GRASS_MAT, FALSE);
	LoadAsset_Material(STONE_MAT, FALSE);
	LoadAsset_Material(SAND_MAT, FALSE);
	LoadAsset_Material(GRASS_EDGE_MAT, FALSE);
	LoadAsset_Material(PATH_MAT, FALSE);
	LoadAsset_Material(DISTANTTREE_MAT, FALSE);
	LoadAsset_Material(GRASSBORDER_MAT, FALSE);

	// Set collision data
	REGISTER_COLLISION_MESH(scene, model_asset, "Shore-Grass", COL_LAYER_WORLD | COL_LAYER_CAMERA);
	REGISTER_COLLISION_MESH(scene, model_asset, "Shore-Cliff", COL_LAYER_WORLD | COL_LAYER_CAMERA);
	REGISTER_COLLISION_MESH(scene, model_asset, "Shore-Sand", COL_LAYER_WORLD | COL_LAYER_CAMERA);
	REGISTER_COLLISION_MESH(scene, model_asset, "Shore-Coast", COL_LAYER_WORLD | COL_LAYER_CAMERA);
}

SCENE_ACTIVATE_ROOM(lakeside_coast)
{
	LoadCustomLayer(scene, IsDay() ? "Day" : "Night");
}

SCENE_PREPARE_ACTORS(lakeside_coast)
{

}

SCENE_DRAWWORLD(lakeside_coast)
{
	Asset* model_asset = AssetGetPackage(FIELD_MODEL);
	STANDARD_SHADER_DRAW(scene, model_asset, GRASS_MAT, "Shore-Grass");
	STANDARD_SHADER_DRAW(scene, model_asset, STONE_MAT, "Shore-Cliff");
	STANDARD_SHADER_DRAW(scene, model_asset, SAND_MAT, "Shore-Sand");
	STANDARD_SHADER_DRAW(scene, model_asset, GRASS_EDGE_MAT, "Shore-Coast");
	STANDARD_SHADER_DRAW(scene, model_asset, PATH_MAT, "Shore-Path");
	STANDARD_SHADER_DRAW(scene, model_asset, DISTANTTREE_MAT, "Shore-DistantTrees");
	STANDARD_SHADER_DRAW(scene, model_asset, GRASSBORDER_MAT, "Shore-GrassBorder");
}