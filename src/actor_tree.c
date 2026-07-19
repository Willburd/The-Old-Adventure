#include "actor_factory.h"
#include "tools.h"
#include "collision.h"
#include "game_draw.h"

// Assets
#define TREE_MODEL ASSET_MODELS"/Scenes/test_room.glb"
#define TREE_MATERIAL_BARK ASSET_MATERIALS"/Objects/wood.mat"
#define TREE_MATERIAL_BRANCHES ASSET_MATERIALS"/Objects/wood.mat"
#define TREE_MATERIAL_LEAVES ASSET_MATERIALS"/Objects/wood.mat"

// Utility
#define TREE_MESH_MAIN 0
#define TREE_MESH_COLLISION 1

// private header
ACTOR_PRELOADASSETS(tree);
ACTOR_DRAWWORLD(tree);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(tree)
{
	actor->actor_flags = 0;
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
	CollisionRegister(actor, &model_asset->mdl->meshes[TREE_MESH_COLLISION], COL_LAYER_WORLD);
}

ACTOR_DRAWWORLD(tree)
{
	if (OutOfRenderRange(actor))
		return;
	Material* mat = AssetGet_Material(TREE_MATERIAL_BARK);
	shader_update_fog(mat->shader);
	shader_update_lights(mat->shader);

	ToaDrawMesh(
		AssetGet_Model(TREE_MODEL)->meshes[TREE_MESH_MAIN],
		*mat,
		GetMatrix(actor),
		FALSE
	);
}