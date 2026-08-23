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
static const char* loaded_materials[] = {
	NULL,									// Room 0: reference cube
	ASSET_MATERIALS"/Natural/stone_B.mat"	// Room 0: Walls
};

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
	Asset* mdl_asset = LoadAsset_Model(TESTROOM_MODEL, FALSE);
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));

	// Set collision data
	RegisterAllCollisionMeshes(scene, TESTROOM_MODEL, COL_LAYER_WORLD | COL_LAYER_CAMERA);
	RESIGN_COLLISION_MESH(scene, mdl_asset, "ref_cube-Material.001");
}

SCENE_ACTIVATE_ROOM(test)
{

}

SCENE_DRAWWORLD(test)
{
	Asset* model_asset = AssetGetPackage(TESTROOM_MODEL);
	STANDARD_SHADER_MATERIAL(stone_mat, loaded_materials[1], scene);

	switch (scene->current_room_index)
	{
		case 0:
		{
			ToaDrawMesh(
				model_asset,
				GetMeshIndex(model_asset->mesh_data, "test_room-Main"),
				*stone_mat,
				GetMatrix(scene),
				FALSE
			);
		}
		break;

		case 1:
		{
			ToaDrawMesh(
				model_asset,
				GetMeshIndex(model_asset->mesh_data, "side_room-Main"),
				*stone_mat,
				GetMatrix(scene),
				FALSE
			);
		}
		break;
	}
}