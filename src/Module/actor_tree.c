#include "../actor_factory.h"
#include "../tools.h"
#include "../collision.h"
#include "../game_draw.h"

// Assets
#define TREE_MODEL ASSET_MODELS"/Trees/tree_A.glb"

#define BARK_MATERIAL ASSET_MATERIALS"/Trees/bark_A.mat"
#define MULCH_MATERIAL ASSET_MATERIALS"/Trees/mulch_A.mat"
#define BRANCH_MATERIAL_PREFIX ASSET_MATERIALS"/Trees/branches_"

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

	// Load Materials
	LoadAsset_Material(BARK_MATERIAL, FALSE);
	LoadAsset_Material(MULCH_MATERIAL, FALSE);
	LoadAsset_Material(BRANCH_MATERIAL_PREFIX"A.mat", FALSE);

	// Set collision data
	REGISTER_COLLISION_MESH(actor, model_asset, DEFAULT_COLLISION_MESH, COL_LAYER_WORLD);
}

ACTOR_DRAWWORLD(tree)
{
	if (OutOfRenderRange(actor))
		return;
	Asset* model_asset = AssetGetPackage(TREE_MODEL);
	STANDARD_SHADER_DRAW(actor, model_asset, BARK_MATERIAL, "Tree-Bark", tick_percent);
	STANDARD_SHADER_DRAW(actor, model_asset, MULCH_MATERIAL, "Tree-Mulch", tick_percent);
	STANDARD_SHADER_DRAW(actor, model_asset, BRANCH_MATERIAL_PREFIX"A.mat", "Tree-Branches", tick_percent);
}