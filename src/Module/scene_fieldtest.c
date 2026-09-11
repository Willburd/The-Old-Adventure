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
static const char* loaded_materials[] = {
	ASSET_MATERIALS"/Natural/grass_A.mat",			// room 0: Grass
	ASSET_MATERIALS"/Objects/wood_door_a.mat",		// room 0: Misc Structure
	ASSET_MATERIALS"/Natural/stone_B.mat",			// room 0: Stone Walls
	ASSET_MATERIALS"/Construction/railway_A.mat"	// room 0: Railway
};

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
	LoadAsset_Model(FIELD_MODEL, FALSE);
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));

	// Set collision data
	RegisterAllCollisionMeshes(scene, FIELD_MODEL, COL_LAYER_WORLD | COL_LAYER_CAMERA);
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
	DrawAllModelMeshes(scene, FIELD_MODEL, loaded_materials);
}