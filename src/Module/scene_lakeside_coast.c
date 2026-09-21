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
#define STONE_MAT ASSET_MATERIALS"/Natural/stone_A.mat"
#define GRAVEL_MAT ASSET_MATERIALS"/Natural/gravel_A.mat"

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
	LoadAsset_Model(FIELD_MODEL, FALSE);
	
	// Load Materials
	LoadAsset_Material(GRASS_MAT, FALSE);
	LoadAsset_Material(STONE_MAT, FALSE);
	LoadAsset_Material(GRAVEL_MAT, FALSE);

	// Set collision data
	RegisterAllCollisionMeshes(scene, FIELD_MODEL, COL_LAYER_WORLD | COL_LAYER_CAMERA);
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

	STANDARD_SHADER_MATERIAL(grass_mat, GRASS_MAT, scene);
	STANDARD_SHADER_MATERIAL(stone_mat, STONE_MAT, scene);
	STANDARD_SHADER_MATERIAL(gravel_mat, GRAVEL_MAT, scene);

	ToaDrawMesh(
		model_asset,
		GetMeshIndex(model_asset->mesh_data, "Shore-Grass"),
		*grass_mat,
		GetMatrix(scene)
	);
	ToaDrawMesh(
		model_asset,
		GetMeshIndex(model_asset->mesh_data, "Shore-Cliff"),
		*stone_mat,
		GetMatrix(scene)
	);
	ToaDrawMesh(
		model_asset,
		GetMeshIndex(model_asset->mesh_data, "Shore-Sand"),
		*gravel_mat,
		GetMatrix(scene)
	);
}