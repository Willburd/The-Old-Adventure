#include "../actor_factory.h"
#include "../tools.h"
#include "../collision.h"
#include "../game_draw.h"
#include "json_properties.h"
#include "adv_utility.h"

#define WOBBLE_DURATION 70

// Assets
#define TREE_MODEL_PREFIX ASSET_MODELS"/Trees/tree_"

#define BARK_MATERIAL ASSET_MATERIALS"/Trees/bark_A.mat"
#define MULCH_MATERIAL ASSET_MATERIALS"/Trees/mulch_A.mat"
#define BRANCH_MATERIAL_PREFIX ASSET_MATERIALS"/Trees/branches_"

// private header
ACTOR_JSON_INIT(tree);
ACTOR_PRELOADASSETS(tree);
ACTOR_UPDATE(tree);
ACTOR_PLAYER_INTERACT(tree);
ACTOR_DRAWWORLD(tree);

typedef struct
{
	unsigned int wobble_counter;
	int has_dropped_items;
} TreeData;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(tree)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_TRANSITION;
	actor->draw_range = DEFAULT_MAX_RENDER_RANGE * 0.80f;
	ACTOR_REGISTER_JSON_INIT(tree);
	ACTOR_REGISTER_PRELOADASSETS(tree);
	ACTOR_REGISTER_UPDATE(tree);
	ACTOR_REGISTER_PLAYER_INTERACT(tree);
	ACTOR_REGISTER_DRAWWORLD(tree);

	// Set data
	MALLOC_ACTOR_DATA(TreeData, actor->data);
	TreeData* tree_data = actor->data;
	tree_data->wobble_counter = 0;
	tree_data->has_dropped_items = FALSE;
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

ACTOR_UPDATE(tree)
{
	TreeData* tree_data = actor->data;
	if (OutOfRenderRange(actor))
	{
		tree_data->wobble_counter = 0;
		return;
	}
	if (tree_data->wobble_counter == 0)
		return;
	// Wobble animation
	actor->rotation = WobbleRotation(actor->home_rotation, ++tree_data->wobble_counter, WOBBLE_DURATION, 1.5f);
	// End wobble
	if (tree_data->wobble_counter < WOBBLE_DURATION)
		return;
	tree_data->wobble_counter = 0;
	actor->rotation = actor->home_rotation;
}

ACTOR_PLAYER_INTERACT(tree)
{
	// Bonking trees
	TreeData* tree_data = actor->data;
	tree_data->wobble_counter = 1;
	// Drop items
	if (tree_data->has_dropped_items)
		return;
	tree_data->has_dropped_items = TRUE;

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