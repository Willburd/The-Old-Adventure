#include "../actor_factory.h"
#include "../tools.h"
#include "../collision.h"
#include "../game_draw.h"

/*******************************************
				Crate
None-specific crate actor. Can be broken to
provide the player with resources. Comes in
multiple sizes based on the scale of the actor.
********************************************/

// Assets
#define CRATE_MODEL ASSET_MODELS"/Objects/crate.glb"
static const char* loaded_materials[] = {
	ASSET_MATERIALS"/Objects/crate_wood_A.mat" // Crate
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
	actor->draw_range = DEFAULT_MEDIUM_RENDER_RANGE;
	actor->collision_range = DEFAULT_SMALL_COLLISION_RANGE;
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

	// Load Materials
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));

	// Set collision data
	RegisterAllCollisionMeshes(actor, CRATE_MODEL, COL_LAYER_WORLD | COL_LAYER_CAMERA);
}

ACTOR_DRAWWORLD(crate)
{
	DrawAllModelMeshes(actor, CRATE_MODEL, loaded_materials, tick_percent);
}