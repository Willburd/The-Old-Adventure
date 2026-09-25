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
	LoadAsset_Model(FIELD_MODEL, FALSE);
	
	// Load Materials
	LoadAsset_Material(GRASS_MAT, FALSE);
	LoadAsset_Material(STONE_MAT, FALSE);
	LoadAsset_Material(SAND_MAT, FALSE);
	LoadAsset_Material(GRASS_EDGE_MAT, FALSE);
	LoadAsset_Material(PATH_MAT, FALSE);
	LoadAsset_Material(DISTANTTREE_MAT, FALSE);
	LoadAsset_Material(GRASSBORDER_MAT, FALSE);

	// Set collision data
	Asset* model_asset = AssetGetPackage(FIELD_MODEL);
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

	STANDARD_SHADER_MATERIAL(grass_mat, GRASS_MAT, scene);
	STANDARD_SHADER_MATERIAL(stone_mat, STONE_MAT, scene);
	STANDARD_SHADER_MATERIAL(gravel_mat, SAND_MAT, scene);
	STANDARD_SHADER_MATERIAL(grass_edge_mat, GRASS_EDGE_MAT, scene);
	STANDARD_SHADER_MATERIAL(path_mat, PATH_MAT, scene);
	STANDARD_SHADER_MATERIAL(distanttree_mat, DISTANTTREE_MAT, scene);
	STANDARD_SHADER_MATERIAL(grass_border_mat, GRASSBORDER_MAT, scene);

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
	ToaDrawMesh(
		model_asset,
		GetMeshIndex(model_asset->mesh_data, "Shore-Coast"),
		*grass_edge_mat,
		GetMatrix(scene)
	);
	ToaDrawMesh(
		model_asset,
		GetMeshIndex(model_asset->mesh_data, "Shore-Path"),
		*path_mat,
		GetMatrix(scene)
	);
	ToaDrawMesh(
		model_asset,
		GetMeshIndex(model_asset->mesh_data, "Shore-DistantTrees"),
		*distanttree_mat,
		GetMatrix(scene)
	);
	ToaDrawMesh(
		model_asset,
		GetMeshIndex(model_asset->mesh_data, "Shore-GrassBorder"),
		*grass_border_mat,
		GetMatrix(scene)
	);

	
}