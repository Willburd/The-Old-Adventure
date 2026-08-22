#include "actor_factory.h"
#include "tools.h"
#include "collision.h"
#include "game_draw.h"

// Assets
#define CRATE_MODEL ASSET_MODELS"/Objects/crate.glb"
static const char* loaded_materials[] = {
	ASSET_MATERIALS"/Objects/wood_crate_a.mat"
};

// private header
ACTOR_PRELOADASSETS(crate);
ACTOR_DRAWWORLD(crate);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(crate)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
	actor->blend_color = ColorToVector4(GOLD);
	ACTOR_REGISTER_PRELOADASSETS(crate);
	ACTOR_REGISTER_DRAWWORLD(crate);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(crate)
{
	// Load model
	LoadAsset_Model(CRATE_MODEL, FALSE);
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));

	// Set collision data
	RegisterAllCollisionMeshes(actor, CRATE_MODEL, COL_LAYER_WORLD | COL_LAYER_CAMERA);
}

ACTOR_DRAWWORLD(crate)
{
	if (OutOfRenderRange(actor))
		return;
	DrawAllModelMeshes(actor, CRATE_MODEL, loaded_materials);
}