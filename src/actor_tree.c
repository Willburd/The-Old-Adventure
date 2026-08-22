#include "actor_factory.h"
#include "tools.h"
#include "collision.h"
#include "game_draw.h"

// Assets
#define TREE_MODEL ASSET_MODELS"/Trees/tree_A.glb"
static const char* loaded_materials[] = {
	ASSET_MATERIALS"/Trees/tree_leaves_a.mat",
	NULL, // collision mesh
	ASSET_MATERIALS"/Trees/tree_branches_a.mat",
	ASSET_MATERIALS"/Trees/tree_bark_a.mat"
};

// private header
ACTOR_PRELOADASSETS(tree);
ACTOR_DRAWWORLD(tree);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(tree)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
	actor->blend_color = ColorToVector4(GOLD);
	ACTOR_REGISTER_PRELOADASSETS(tree);
	ACTOR_REGISTER_DRAWWORLD(tree);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(tree)
{
	// Load model
	Asset* model_asset = LoadAsset_Model(TREE_MODEL, FALSE);
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));

	// Set collision data
	REGISTER_COLLISION_MESH(actor, model_asset, DEFAULT_COLLISION_MESH, COL_LAYER_WORLD);
}

ACTOR_DRAWWORLD(tree)
{
	if (OutOfRenderRange(actor))
		return;
	DrawAllModelMeshes(actor, TREE_MODEL, loaded_materials);
}