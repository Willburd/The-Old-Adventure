#include "../actor_factory.h"
#include "../tools.h"
#include "../collision.h"
#include "../game_draw.h"
#include "core_assets.h"
#include "actor_water_volume.h"

// Assets
#define WATER_MODEL ASSET_MODELS"/Objects/water_volume.glb"

// private header
ACTOR_PRELOADASSETS(water_volume);
ACTOR_DRAWWORLD(water_volume);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(water_volume)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
	ACTOR_REGISTER_PRELOADASSETS(water_volume);
	ACTOR_REGISTER_DRAWWORLD(water_volume);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(water_volume)
{
	// Load model
	LoadAsset_Model(WATER_MODEL, FALSE);
}

ACTOR_DRAWWORLD(water_volume)
{
	Asset* model_asset = AssetGetPackage(WATER_MODEL);
	STANDARD_SHADER_DRAW(actor, model_asset, ASSET_MATERIALS"/Engine/example.mat", "Surface");
}