#include "../actor_factory.h"
#include "../tools.h"
#include "../collision.h"
#include "../game_draw.h"

// Assets
#define BUSH_MODEL ASSET_MODELS"/Trees/bush_A.glb"
static const char* loaded_materials[] = {
	ASSET_MATERIALS"/Trees/branches_B.mat" // Branches
};

// private header
ACTOR_PRELOADASSETS(bush);
ACTOR_DRAWWORLD(bush);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(bush)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
	actor->blend_color = ColorToVector4(GOLD);
	ACTOR_REGISTER_PRELOADASSETS(bush);
	ACTOR_REGISTER_DRAWWORLD(bush);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(bush)
{
	// Load model
	Asset* model_asset = LoadAsset_Model(BUSH_MODEL, FALSE);

	// Load Materials
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));
}

ACTOR_DRAWWORLD(bush)
{
	if (OutOfRenderRange(actor))
		return;
	DrawAllModelMeshes(actor, BUSH_MODEL, loaded_materials, tick_percent);
}