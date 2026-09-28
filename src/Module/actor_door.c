#include "../tools.h"
#include "../assets.h"
#include "../actor_factory.h"
#include "../actor_scene.h"
#include "../game_draw.h"
#include "../collision.h"

// Assets
#define DOOR_MODEL ASSET_MODELS"/Objects/door.glb"
static const char* loaded_materials[] = {
	ASSET_MATERIALS"/Objects/door_wood_A.mat"
};

// private header
ACTOR_JSON_INIT(door);
ACTOR_PRELOADASSETS(door);
ACTOR_DRAWWORLD(door);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(door)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
	ACTOR_REGISTER_PRELOADASSETS(door);
	ACTOR_REGISTER_JSON_INIT(door);
	ACTOR_REGISTER_DRAWWORLD(door);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_JSON_INIT(door)
{
	if (file_data == NULL)
		return;

}

ACTOR_PRELOADASSETS(door)
{
	// Load model
	LoadAsset_Model(DOOR_MODEL, FALSE);
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));

	// Set collision data
	RegisterAllCollisionMeshes(actor, DOOR_MODEL, COL_LAYER_WORLD | COL_LAYER_CAMERA);
}

ACTOR_DRAWWORLD(door)
{
	if (OutOfRenderRange(actor))
		return;
	DrawAllModelMeshes(actor, DOOR_MODEL, loaded_materials);
}