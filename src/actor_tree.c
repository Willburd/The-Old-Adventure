#include "actor_factory.h"
#include "tools.h"
#include "collision.h"
#include "game_draw.h"

// Assets
#define TREE_MODEL ASSET_MODELS"/Trees/tree_A.glb"
#define TREE_MATERIAL_BARK ASSET_MATERIALS"/Trees/tree_bark_a.mat"
#define TREE_MATERIAL_BRANCHES ASSET_MATERIALS"/Trees/tree_branches_a.mat"
#define TREE_MATERIAL_LEAVES ASSET_MATERIALS"/Trees/tree_leaves_a.mat"

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
	LoadAsset_Material(TREE_MATERIAL_BARK, FALSE);
	LoadAsset_Material(TREE_MATERIAL_BRANCHES, FALSE);
	LoadAsset_Material(TREE_MATERIAL_LEAVES, FALSE);

	// Set collision data
	REGISTER_COLLISION_MESH(actor, model_asset, DEFAULT_COLLISION_MESH, COL_LAYER_WORLD);
}

ACTOR_DRAWWORLD(tree)
{
	if (OutOfRenderRange(actor))
		return;
	Asset* model_asset = AssetGetPackage(TREE_MODEL);
	Matrix position = GetMatrix(actor);

	STANDARD_SHADER_MATERIAL(bark_mat, TREE_MATERIAL_BARK, actor);
	STANDARD_SHADER_MATERIAL(branches_mat, TREE_MATERIAL_BRANCHES, actor);
	STANDARD_SHADER_MATERIAL(leaves_mat, TREE_MATERIAL_LEAVES, actor);

	int trunk_mesh_index = GetMeshIndex(model_asset->mesh_data, "Tree-Bark");
	int branch_mesh_index = GetMeshIndex(model_asset->mesh_data, "Tree-Branches");
	int leaves_mesh_index = GetMeshIndex(model_asset->mesh_data, "Tree-Leaves");
	ToaDrawMesh(
		model_asset,
		trunk_mesh_index,
		*bark_mat,
		position,
		FALSE
	);
	ToaDrawMesh(
		model_asset,
		branch_mesh_index,
		*branches_mat,
		position,
		TRUE
	);
	ToaDrawMesh(
		model_asset,
		leaves_mesh_index,
		*leaves_mat,
		position,
		TRUE
	);
}