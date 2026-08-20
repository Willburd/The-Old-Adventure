#include "actor_factory.h"
#include "tools.h"
#include "collision.h"
#include "game_draw.h"
#include "core_assets.h"
#include <math.h>

// Assets
#define ROCK_MODEL ASSET_MODELS"/Objects/smallrock.glb"
#define ROCK_MATERIAL ASSET_MATERIALS"/Natural/stone_B.mat"

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
	Asset* model_asset = LoadAsset_Model(ROCK_MODEL, FALSE);
	LoadAsset_Material(ROCK_MATERIAL, FALSE);

	// Set collision data
	REGISTER_COLLISION_MESH(actor, AssetGetPackage(SIMPLE_ACTOR_COLLISION_MODEL), DEFAULT_COLLISION_MESH, COL_LAYER_WORLD);
}

ACTOR_DRAWWORLD(smallrock)
{
	if (OutOfRenderRange(actor))
		return;
	Asset* model_asset = AssetGetPackage(ROCK_MODEL);
	Matrix position = GetMatrix(actor);

	STANDARD_SHADER_MATERIAL(rock_mat, ROCK_MATERIAL, actor);

	int rock_mesh_index = GetMeshIndex(model_asset->mesh_data, "Rock-Main");
	ToaDrawMesh(
		model_asset,
		rock_mesh_index,
		*rock_mat,
		position,
		FALSE
	);
}