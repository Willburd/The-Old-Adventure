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
ACTOR_PREUPDATE(platform);
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
	ACTOR_REGISTER_PREUPDATE(platform);
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

ACTOR_PREUPDATE(platform)
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
	NodeData* target_data = target_goal->data;
	Vector3 dir = VEC3DIRECTION(actor->position, target_goal->position);
	float remaining_distance = Vector3Distance(actor->position, target_goal->position) - platform_data->speed;
	actor->velocity = Vector3Scale(dir, min(platform_data->speed, max(remaining_distance, 0.0f)));

	// Next node time! Snap if we are near enough.
	if (remaining_distance >= 0.01)
		return;
	actor->position = target_goal->position;
	actor->rotation = target_goal->rotation;
	actor->velocity = Vector3Zero();
	actor->angular_velocity = Vector3Zero();
	remaining_distance = 0;

	// Check if the next node exists
	if (target_data->next_node_tag == NULL)
		return;
	// Transfer node id
	struct Actor* next_node = FINDACTOR_BYTAG(target_data->next_node_tag);
	if (next_node == NULL)
		return;
	RELEASE(platform_data->target_node);
	CHAR_STR_COPY(platform_data->target_node, next_node->id_tag, NULL);

	// Set the initial velocity
	actor->velocity = Vector3Scale(VEC3DIRECTION(actor->position, next_node->position), platform_data->speed);
	// Set the rotation, it should automatically rotate into final position when it arrives!
	float total_distance = Vector3Distance(actor->position, next_node->position);
	float rotation_mult = platform_data->speed / total_distance;
	Vector3 current_eulars = QuaternionToEuler(actor->rotation);
	Vector3 goal_eulars = QuaternionToEuler(next_node->rotation);
	actor->angular_velocity = (Vector3){ // TODO - Rotations here aren't behaving correctly when chained multiple times...
											(float)AngleDifference(current_eulars.z, goal_eulars.z) * -rotation_mult, 
											(float)AngleDifference(current_eulars.y, goal_eulars.y) * -rotation_mult,
											(float)AngleDifference(current_eulars.x, goal_eulars.x) * -rotation_mult
										};
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