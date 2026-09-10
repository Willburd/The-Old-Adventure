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
ACTOR_PRELOADASSETS(woodtorch);
ACTOR_JSON_INIT(woodtorch);
ACTOR_DRAWWORLD(woodtorch);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(woodtorch)
{
	actor->actor_flags = 0;
	ACTOR_REGISTER_PRELOADASSETS(woodtorch);
	ACTOR_REGISTER_JSON_INIT(woodtorch);
	ACTOR_REGISTER_DRAWWORLD(woodtorch);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(woodtorch)
{
	// Load model
	LoadAsset_Model(WOODTORCH_MODEL, FALSE);
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));

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
	int burned_out = JSON_GET_BOOL(file_data, PROP_BURNEDOUT);
	if(!burned_out)
		ACTOR_FACTORY(NULL, act_fire, actor, Vector3RotateByQuaternion(Vector3Add(actor->position, Vector3Scale(VEC3UP, 3.0f)), actor->rotation), QuaternionIdentity(), Vector3One(), Vector3Zero(), Vector3Zero());
}

ACTOR_DRAWWORLD(woodtorch)
{
	if (OutOfRenderRange(actor))
		return;
	DrawAllModelMeshes(actor, WOODTORCH_MODEL, loaded_materials);
}