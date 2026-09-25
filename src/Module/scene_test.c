#include <stdio.h>
#include <stdlib.h>
#include "../tools.h"
#include "../actor_factory.h"
#include "../actor_scene.h"
#include "../scene_entry.h"
#include "../collision.h"
#include "../game_draw.h"

/*******************************************
			Gameplay Debuging Room
********************************************/

// Assets
#define TESTROOM_MODEL ASSET_MODELS"/Scenes/test_room.glb"
#define STONE_MAT ASSET_MATERIALS"/Natural/stone_B.mat"

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

	// Load Materials
	LoadAsset_Material(STONE_MAT, FALSE);

	// Set collision data
	REGISTER_COLLISION_MESH(scene, model_asset, "test_room-Main", COL_LAYER_WORLD | COL_LAYER_CAMERA);
	REGISTER_COLLISION_MESH(scene, model_asset, "side_room-Main", COL_LAYER_WORLD | COL_LAYER_CAMERA);
}

SCENE_ACTIVATE_ROOM(test)
{

}

SCENE_DRAWWORLD(test)
{
	Asset* model_asset = AssetGetPackage(TESTROOM_MODEL);
	switch (scene->current_room_index)
	{
		case 0:
			STANDARD_SHADER_DRAW(scene, model_asset, STONE_MAT, "test_room-Main");
			break;

		case 1:
			STANDARD_SHADER_DRAW(scene, model_asset, STONE_MAT, "side_room-Main");
			break;
	}
}