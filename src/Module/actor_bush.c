#include "../actor_factory.h"
#include "../tools.h"
#include "../collision.h"
#include "../game_draw.h"

// Assets
#define BUSH_MODEL ASSET_MODELS"/Trees/bush_A.glb"
#define BRANCH_MATERIAL_PREFIX ASSET_MATERIALS"/Trees/branches_"

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
	LoadAsset_Material(ALTSKIN(BRANCH_MATERIAL_PREFIX, actor->skin_select, ".mat"), FALSE);
}

ACTOR_DRAWWORLD(bush)
{
	if (OutOfRenderRange(actor))
		return;
	Asset* model_asset = AssetGetPackage(BUSH_MODEL);
	STANDARD_SHADER_DRAW(actor, model_asset, ALTSKIN(BRANCH_MATERIAL_PREFIX, actor->skin_select, ".mat"), "Bush-Branches", tick_percent);
}