#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_scene.h"
#include "assets.h"
#include "collision.h"

/*******************************************
		   Large Playspace Testing
********************************************/

// Assets
#define FIELD_MODEL ASSET_MODELS"/Scenes/test_cave.glb"
static const char* loaded_materials[] = {
	ASSET_MATERIALS"/Natural/stone_B.mat",			// room 0: Stone Walls
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
	LoadAsset_Model(FIELD_MODEL, FALSE);
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));

	// Set collision data
	RegisterAllCollisionMeshes(scene, FIELD_MODEL, COL_LAYER_WORLD | COL_LAYER_CAMERA);
}

SCENE_DRAWWORLD(cavetest)
{
	DrawAllModelMeshes(scene, FIELD_MODEL, loaded_materials);
}