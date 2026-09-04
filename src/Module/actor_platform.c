#include "../actor_factory.h"
#include "../tools.h"
#include "../collision.h"
#include "../game_draw.h"

// Assets
#define PLATFORM_MODEL ASSET_MODELS"/Objects/simple_platform.glb"
static const char* loaded_materials[] = {
	ASSET_MATERIALS"/Construction/roughblock_A.mat", // stonework
};

// private header
ACTOR_PRELOADASSETS(platform);
ACTOR_DRAWWORLD(platform);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(platform)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
	ACTOR_REGISTER_PRELOADASSETS(platform);
	ACTOR_REGISTER_DRAWWORLD(platform);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(platform)
{
	// Load model
	Asset* model_asset = LoadAsset_Model(PLATFORM_MODEL, FALSE);
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));

	// Set collision data
	REGISTER_COLLISION_MESH(actor, model_asset, DEFAULT_COLLISION_MESH, COL_LAYER_WORLD | COL_LAYER_MOVINGPLATFORM | COL_LAYER_CAMERA);
}

ACTOR_DRAWWORLD(platform)
{
	if (OutOfRenderRange(actor))
		return;
	DrawAllModelMeshes(actor, PLATFORM_MODEL, loaded_materials);
}