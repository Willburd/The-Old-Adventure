#include "../actor_factory.h"
#include "../tools.h"
#include "../collision.h"
#include "../game_draw.h"
#include "actor_platform.h"
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
	float percent;
	Vector3 last_node_pos;
	Quaternion last_node_rot;
	Vector3 start_pos;
	int is_moving;
	int is_reversed;
} PlatformData;
ACTOR_PRELOADASSETS(platform);
ACTOR_JSON_INIT(platform);
ACTOR_REMOTE_INTERACT(platform);
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
	ACTOR_REGISTER_REMOTE_INTERACT(platform);
	ACTOR_REGISTER_PREUPDATE(platform);
	ACTOR_REGISTER_CLEANUP(platform);
	ACTOR_REGISTER_DRAWWORLD(platform);
}

void InitPlatformData(struct Actor* actor, float speed, int is_moving, int is_reversed)
{
	PlatformData* platform_data = (PlatformData*)actor->data;
	platform_data->target_node = NULL;
	platform_data->speed = speed;
	platform_data->percent = 0.0f;
	platform_data->last_node_pos = actor->position;
	platform_data->last_node_rot = actor->rotation;
	platform_data->start_pos = actor->position;
	platform_data->is_moving = is_moving;
	platform_data->is_reversed = is_reversed;
}

void InitPlatformJson(struct Actor* actor, cJSON* file_data)
{
	PlatformData* platform_data = (PlatformData*)actor->data;
	JSON_GET_STRING(platform_data->target_node, file_data, PROP_PLATFORM_STARTNODE, NULL);
	JSON_GET_FLOAT(platform_data->speed, file_data, PROP_PLATFORM_PATHSPEED, 0.0f);
	platform_data->is_moving = JSON_GET_BOOL(file_data, PROP_PLATFORM_STARTMOVING);
}

void HandlePlatformMove(struct Actor* actor)
{
	PlatformData* platform_data = (PlatformData*)actor->data;
	if (!platform_data->is_moving)
		return;
	if (platform_data->speed == 0)
		return;
	if (platform_data->target_node == NULL)
		return;
	struct Actor* target_goal = FINDACTOR_BYTAG(platform_data->target_node);
	if (target_goal == NULL)
	{
		RELEASE(platform_data->target_node); // The target might of been deleted, just give up and deallocate it
		return;
	}

	// Move to the current target
	float distance = Vector3Distance(platform_data->last_node_pos, target_goal->position);
	float move_percent = 1.0f; // Assume we've arrived at the end node so that we...
	if (distance > 0.0f) // Prevent dividing by 0 on instant snap nodes.
		move_percent = platform_data->speed / distance;

	// Advance platform along the path ahead
	float current_percent = platform_data->percent;
	platform_data->percent = min(platform_data->percent + move_percent, 1.0f);
	if (current_percent < 1.0f)
	{
		// Put the platform at the current position it should be
		actor->position = Vector3Lerp(platform_data->last_node_pos, target_goal->position, current_percent);
		Vector3 new_position = Vector3Lerp(platform_data->last_node_pos, target_goal->position, platform_data->percent);
		actor->rotation = QuaternionSlerp(platform_data->last_node_rot, target_goal->rotation, current_percent);
		Quaternion new_rotation = QuaternionSlerp(platform_data->last_node_rot, target_goal->rotation, platform_data->percent);

		// Apply velocity for the movement with the difference of the current position to the goal next frame!
		actor->velocity = Vector3Subtract(new_position, actor->position);
		actor->angular_velocity = QuaternionToEuler(QuaternionMultiply(new_rotation, QuaternionInvert(actor->rotation))); // Get the difference
		return;
	}

	// Next node time! Snap!
	RELEASE(platform_data->target_node); // Release before we find out if the next node exists
	InitPlatformData(actor, platform_data->speed, platform_data->is_moving, platform_data->is_reversed);

	// Check if the next node exists
	NodeData* target_data = target_goal->data;
	if (target_data->next_node_tag == NULL)
		return;

	// Trigger node linked actions
	if (target_data->arrival_triggers_tag != NULL)
	{
		struct Actor* trigger_actor = FINDACTOR_BYTAG(target_data->arrival_triggers_tag);
		if (trigger_actor != NULL && ACTOR_HAS(trigger_actor, func_remote_interact))
			trigger_actor->func_remote_interact(trigger_actor, actor);
	}

	// Set behavior for platform
	if (target_data->node_action == NODEACTION_STOP)
		platform_data->is_moving = FALSE;
	if (target_data->node_action == NODEACTION_REVERSE)
		platform_data->is_reversed = FALSE;
	if (target_data->node_action == NODEACTION_NEXT)
	{
		// Transfer node id
		struct Actor* next_node = FINDACTOR_BYTAG(!platform_data->is_reversed ? target_data->next_node_tag : target_data->prev_node_tag);
		if (next_node == NULL)
			return;
		// Allocate the next goal now that we know it exists
		CHAR_STR_COPY(platform_data->target_node, next_node->id_tag, NULL);
	}
}

void ApplyPlatformRotation(struct Actor* actor, struct Actor* platform, int influence_rotation)
{
	Vector3 offset_position = Vector3Subtract(actor->position, platform->position);
	Vector3 rotation_delta = platform->angular_velocity;
	Quaternion rotation_quat = QuaternionFromEuler(rotation_delta.x, rotation_delta.y, rotation_delta.z);
	offset_position = Vector3RotateByQuaternion(offset_position, rotation_quat);
	actor->position = Vector3Add(platform->position, offset_position);
	actor->position = Vector3Add(actor->position, platform->velocity);
	if (influence_rotation == TRUE)
		actor->rotation = QuaternionMultiply(actor->rotation, QuaternionGetFlat(rotation_quat, VEC3UP)); // Only topdown rotation.
	else if (influence_rotation == 2) // Alternate, ugly implementation but oh well.
		actor->rotation = QuaternionMultiply(actor->rotation, rotation_quat); // Directly apply it.
}

void HandlePlatformCleanup(struct Actor* actor)
{
	PlatformData* platform_data = (PlatformData*)actor->data;
	RELEASE(platform_data->target_node);
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
	InitPlatformData(actor, 0.0f, FALSE, FALSE);
}

ACTOR_JSON_INIT(platform)
{
	if (file_data == NULL)
		return;
	InitPlatformJson(actor, file_data);
}

ACTOR_REMOTE_INTERACT(platform)
{
	PlatformData* platform_data = (PlatformData*)actor->data;
	platform_data->is_moving = !platform_data->is_moving;
}

ACTOR_PREUPDATE(platform)
{
	HandlePlatformMove(actor);
}

ACTOR_CLEANUP(platform)
{
	HandlePlatformCleanup(actor);
}

ACTOR_DRAWWORLD(platform)
{
	if (OutOfRenderRange(actor))
		return;
	DrawAllModelMeshes(actor, PLATFORM_MODEL, loaded_materials);
}