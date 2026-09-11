#include "../actor_factory.h"
#include "../tools.h"
#include "../collision.h"
#include "../game_draw.h"
#include "../models.h"
#include "json_properties.h"
#include "core_assets.h"

// Assets
#define WOODTORCH_MODEL ASSET_MODELS"/Objects/wood_torch.glb"
static const char* loaded_materials[] = {
	ASSET_MATERIALS"/Objects/wood_door_a.mat" // Torch base
};

// private header
typedef struct {
	uint64_t fire_uuid;
	int torch_burning;
} TorchData;
ACTOR_PRELOADASSETS(woodtorch);
ACTOR_JSON_INIT(woodtorch);
ACTOR_UPDATE(woodtorch);
ACTOR_REMOTE_INTERACT(woodtorch);
ACTOR_DRAWWORLD(woodtorch);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(woodtorch)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
	ACTOR_REGISTER_PRELOADASSETS(woodtorch);
	ACTOR_REGISTER_JSON_INIT(woodtorch);
	ACTOR_REGISTER_UPDATE(woodtorch);
	ACTOR_REGISTER_REMOTE_INTERACT(woodtorch);
	ACTOR_REGISTER_DRAWWORLD(woodtorch);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Vector3 FirePos(struct Actor* actor)
{
	float pos_mod = 1.0f;
	TorchData* torch_data = (TorchData*)actor->data;
	if (torch_data->fire_uuid > 0)
	{
		struct Actor* fire = FINDACTOR_BYID(torch_data->fire_uuid);
		if (fire != NULL)
			pos_mod = VEC3MAGNITUDE(fire->scale);
	}
	return Vector3RotateByQuaternion(Vector3Add(actor->position, Vector3Scale(VEC3UP, 2.0f + pos_mod)), actor->rotation);
}

ACTOR_PRELOADASSETS(woodtorch)
{
	// Load model
	LoadAsset_Model(WOODTORCH_MODEL, FALSE);
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));

	// Set data
	MALLOC_ACTOR_DATA(TorchData, actor->data);
	TorchData* torch_data = (TorchData*)actor->data;
	torch_data->torch_burning = TRUE;
	torch_data->fire_uuid = 0;

	// Set collision data
	REGISTER_COLLISION_MESH(actor, AssetGetPackage(SIMPLE_ACTOR_COLLISION_MODEL), DEFAULT_COLLISION_MESH, COL_LAYER_WORLD);
}

ACTOR_JSON_INIT(woodtorch)
{
	if (file_data == NULL)
		return;

	// Array4 color
	JSON_GET_VECTOR4(actor->blend_color, file_data, PROP_FIRECOLOR, ColorToVector4(GOLD));

	// Spawn child fire object
	TorchData* torch_data = (TorchData*)actor->data;
	torch_data->torch_burning = !JSON_GET_BOOL(file_data, PROP_BURNEDOUT);
	struct Actor* actor_fire = ACTOR_FACTORY(NULL, act_fire, actor, FirePos(actor), QuaternionIdentity(), Vector3One(), Vector3Zero(), Vector3Zero());
	torch_data->fire_uuid = actor_fire->uuid;
	if (!torch_data->torch_burning)
		ACTOR_SCALE_SNAP(actor_fire, Vector3Zero());
}

ACTOR_REMOTE_INTERACT(woodtorch)
{
	TorchData* torch_data = (TorchData*)actor->data;
	torch_data->torch_burning = !torch_data->torch_burning;
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_TEXTBOX | ACTOR_FLAG_TICKDURING_TRANSITION;
}

ACTOR_UPDATE(woodtorch)
{
	TorchData* torch_data = (TorchData*)actor->data;
	if (torch_data->fire_uuid == 0)
		return;
	struct Actor* fire = FINDACTOR_BYID(torch_data->fire_uuid);
	if (fire == NULL)
		return;

	// Shrink and grow fire, then stop updating
	if (!torch_data->torch_burning)
	{
		fire->scale = Vector3Scale(fire->scale, 0.95f); // Shrink flame
		fire->position = FirePos(actor);
		if (VEC3MAGNITUDE(fire->scale) < 0.05f)
		{
			fire->scale = Vector3Zero();
			fire->position = FirePos(actor);
			actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
			return;
		}
	}
	else
	{
		if (VEC3MAGNITUDE(fire->scale) == 0)
			fire->scale = Vector3Scale(Vector3One(), 0.1f);
		fire->scale = Vector3Scale(fire->scale, 1.2f); // Grow flame
		fire->position = FirePos(actor);
		if (VEC3MAGNITUDE(fire->scale) >= 1.0f)
		{
			fire->scale = Vector3One();
			fire->position = FirePos(actor);
			actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
			return;
		}

	}
}

ACTOR_DRAWWORLD(woodtorch)
{
	if (OutOfRenderRange(actor))
		return;
	DrawAllModelMeshes(actor, WOODTORCH_MODEL, loaded_materials);
}