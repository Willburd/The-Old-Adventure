#include "actor_factory.h"
#include "tools.h"
#include "collision.h"
#include "game_draw.h"

// Assets
#define CRATE_MODEL ASSET_MODELS"/Objects/crate.glb"
#define CRATE_MATERIAL ASSET_MATERIALS"/Objects/wood_crate_a.mat"

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
	Asset* model_asset = LoadAsset_Model(CRATE_MODEL, FALSE);
	LoadAsset_Material(CRATE_MATERIAL, FALSE);

	// Set collision data
	REGISTER_COLLISION_MESH(actor, model_asset, "Crate-Main", COL_LAYER_WORLD | COL_LAYER_CAMERA);
}

ACTOR_DRAWWORLD(crate)
{
	if (OutOfRenderRange(actor))
		return;
	Asset* model_asset = LoadAsset_Model(CRATE_MODEL, FALSE);
	Matrix position = GetMatrix(actor);

	STANDARD_SHADER_MATERIAL(crate_mat, CRATE_MATERIAL, actor);

	int trunk_mesh_index = GetMeshIndex(model_asset->mesh_data, "Crate-Main");
	ToaDrawMesh(
		model_asset,
		GetMeshIndex(model_asset->mesh_data, "Crate-Main"),
		*crate_mat,
		position,
		FALSE
	);
}