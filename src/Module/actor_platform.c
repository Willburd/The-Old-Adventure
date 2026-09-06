#include "../actor_factory.h"
#include "../tools.h"
#include "../collision.h"
#include "../game_draw.h"
#include "actor_node.h"
#include "json_properties.h"

// Assets
#define PLATFORM_MODEL ASSET_MODELS"/Objects/simple_platform.glb"
static const char* loaded_materials[] = {
	ASSET_MATERIALS"/Construction/roughblock_A.mat", // stonework
};

// private header
typedef struct
{
	char* target_node;
	float speed;
} PlatformData;
ACTOR_PRELOADASSETS(platform);
ACTOR_JSON_INIT(platform);
ACTOR_UPDATE(platform);
ACTOR_CLEANUP(platform);
ACTOR_DRAWWORLD(platform);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(platform)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
	ACTOR_REGISTER_PRELOADASSETS(platform);
	ACTOR_REGISTER_JSON_INIT(platform);
	ACTOR_REGISTER_UPDATE(platform);
	ACTOR_REGISTER_CLEANUP(platform);
	ACTOR_REGISTER_DRAWWORLD(platform);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(platform)
{
	// Load model
	Asset* model_asset = LoadAsset_Model(PLATFORM_MODEL, FALSE);
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));

	// Set collision data
	REGISTER_COLLISION_MESH(actor, model_asset, DEFAULT_COLLISION_MESH, COL_LAYER_WORLD | COL_LAYER_MOVINGPLATFORM | COL_LAYER_CAMERA);

	// Set data
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME;
	MALLOC_ACTOR_DATA(PlatformData, actor->data);
	PlatformData* platform_data = (PlatformData*)actor->data;
	platform_data->target_node = NULL;
	platform_data->speed = 0.0f;
}

ACTOR_JSON_INIT(platform)
{
	if (file_data == NULL)
		return;

	PlatformData* platform_data = (PlatformData*)actor->data;
	cJSON* get_str = cJSON_GetObjectItem(file_data, PROP_PLATFORM_STARTNODE);
	if (cJSON_IsString(get_str))
	{
		char* str = get_str->valuestring;
		CHAR_STR_COPY(platform_data->target_node, str, NULL);
	}

	cJSON* get_spd = cJSON_GetObjectItem(file_data, PROP_PLATFORM_PATHSPEED);
	if (cJSON_IsNumber(get_spd))
	{
		platform_data->speed = (float)get_spd->valuedouble;
	}
}

ACTOR_UPDATE(platform)
{
	PlatformData* platform_data = (PlatformData*)actor->data;
	if (platform_data->speed == 0)
		return;
	if (platform_data->target_node == NULL)
		return;

	// Move to the current target
	struct Actor* target_goal = FINDACTOR_BYTAG(platform_data->target_node);
	if (target_goal == NULL)
		return;
	Vector3 dir = VEC3DIRECTION(actor->position, target_goal->position);
	float remaining_distance = Vector3Distance(actor->position, target_goal->position) - platform_data->speed;
	actor->velocity = Vector3Scale(dir, min(platform_data->speed, max(remaining_distance, 0.0f)));

	// Snap stop!
	if (remaining_distance < 0.01)
	{
		actor->position = target_goal->position;
		actor->velocity = Vector3Zero();
		remaining_distance = 0;
	}

	// Next node time!
	if (remaining_distance > 0)
		return;
	NodeData* next_node_data = target_goal->data;
	if (next_node_data->next_node_tag == NULL)
		return;
	RELEASE(platform_data->target_node);
	CHAR_STR_COPY(platform_data->target_node, next_node_data->next_node_tag, NULL);
}

ACTOR_CLEANUP(platform)
{
	PlatformData* platform_data = (PlatformData*)actor->data;
	RELEASE(platform_data->target_node);
}

ACTOR_DRAWWORLD(platform)
{
	if (OutOfRenderRange(actor))
		return;
	DrawAllModelMeshes(actor, PLATFORM_MODEL, loaded_materials);
}