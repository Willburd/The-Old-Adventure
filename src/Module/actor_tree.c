#include "../actor_factory.h"
#include "../tools.h"
#include "../collision.h"
#include "../game_draw.h"
#include "json_properties.h"

// Assets
#define TREE_MODEL_PREFIX ASSET_MODELS"/Trees/tree_"

#define BARK_MATERIAL ASSET_MATERIALS"/Trees/bark_A.mat"
#define MULCH_MATERIAL ASSET_MATERIALS"/Trees/mulch_A.mat"
#define BRANCH_MATERIAL_PREFIX ASSET_MATERIALS"/Trees/branches_"

// private header
ACTOR_JSON_INIT(tree);
ACTOR_PRELOADASSETS(tree);
ACTOR_DRAWWORLD(tree);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(tree)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
	actor->blend_color = ColorToVector4(GOLD);
	ACTOR_REGISTER_JSON_INIT(tree);
	ACTOR_REGISTER_PRELOADASSETS(tree);
	ACTOR_REGISTER_DRAWWORLD(tree);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_JSON_INIT(tree)
{
	if (JSON_GET_BOOL(file_data, PROP_RANDOMVARIATION))
	{
		actor->rotation = QuaternionMultiply(actor->rotation,
			QuaternionFromEuler(
				(-1.0f + fmod(GetFixedRandomFloat(actor->position, 712.3f), 2.0f)) * DEG2RAD,
				fmod(GetFixedRandomFloat(actor->position, 2321.4f), 360.0f) * DEG2RAD,
				(-1.0f + fmod(GetFixedRandomFloat(actor->position, 217.3f), 2.0f)) * DEG2RAD));
		actor->scale = Vector3Scale(actor->scale, 0.95f + fmod(GetFixedRandomFloat(actor->position, 821.2f), 0.1f));
	}
}

ACTOR_PRELOADASSETS(tree)
{
	// Load model
	Asset* model_asset = LoadAsset_Model(ALTSKIN(TREE_MODEL_PREFIX, actor->model_select, ".glb"), FALSE);

	// Load Materials
	LoadAsset_Material(BARK_MATERIAL, FALSE);
	LoadAsset_Material(MULCH_MATERIAL, FALSE);
	LoadAsset_Material(ALTSKIN(BRANCH_MATERIAL_PREFIX, actor->skin_select, ".mat"), FALSE);

	// Set collision data
	REGISTER_COLLISION_MESH(actor, model_asset, DEFAULT_COLLISION_MESH, COL_LAYER_WORLD);
}

ACTOR_DRAWWORLD(tree)
{
	if (OutOfRenderRange(actor))
		return;
	Asset* model_asset = AssetGetPackage(ALTSKIN(TREE_MODEL_PREFIX, actor->model_select, ".glb"));
	STANDARD_SHADER_DRAW(actor, model_asset, BARK_MATERIAL, "Tree-Bark", tick_percent);
	STANDARD_SHADER_DRAW(actor, model_asset, MULCH_MATERIAL, "Tree-Mulch", tick_percent);
	STANDARD_SHADER_DRAW(actor, model_asset, ALTSKIN(BRANCH_MATERIAL_PREFIX, actor->skin_select, ".mat"), "Tree-Branches", tick_percent);
}