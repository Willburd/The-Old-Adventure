#include "actor_factory.h"
#include "tools.h"
#include "collision.h"
#include "game_draw.h"
#include "core_assets.h"
#include <math.h>

// Assets
#define ROCK_MODEL ASSET_MODELS"/Objects/smallrock.glb"
static const char* loaded_materials[] = {
	ASSET_MATERIALS"/Natural/stone_B.mat"
};

// private header
ACTOR_PRELOADASSETS(smallrock);
ACTOR_DRAWWORLD(smallrock);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(smallrock)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
	actor->blend_color = ColorToVector4(GOLD);
	ACTOR_REGISTER_PRELOADASSETS(smallrock);
	ACTOR_REGISTER_DRAWWORLD(smallrock);

	// Fake random rotation per rock
	actor->rotation = QuaternionMultiply(actor->rotation, QuaternionFromAxisAngle(VEC3UP, (float)fmod((float)GetFixedRandomFloat(actor->position, 18283), 360.0f * DEG2RAD)));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(smallrock)
{
	// Load model
	LoadAsset_Model(ROCK_MODEL, FALSE);
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));

	// Set collision data
	REGISTER_COLLISION_MESH(actor, AssetGetPackage(SIMPLE_ACTOR_COLLISION_MODEL), DEFAULT_COLLISION_MESH, COL_LAYER_WORLD);
}

ACTOR_DRAWWORLD(smallrock)
{
	if (OutOfRenderRange(actor))
		return;
	DrawAllModelMeshes(actor, ROCK_MODEL, loaded_materials);
}