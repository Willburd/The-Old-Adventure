#include "actor_factory.h"
#include "tools.h"
#include "collision.h"
#include "game_draw.h"

// Assets
#define WOODTORCH_MODEL ASSET_MODELS"/Objects/wood_torch.glb"
#define WOODTORCH_MATERIAL ASSET_MATERIALS"/Objects/wood_torch.mat"

// Utility
#define WOODTORCH_MESH_MAIN 0
#define WOODTORCH_MESH_COLLISION 1

// private header
ACTOR_PRELOADASSETS(woodtorch);
ACTOR_CLEANUP(woodtorch);
ACTOR_DRAWWORLD(woodtorch);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(woodtorch)
{
	actor->actor_flags = 0;
	actor->blend_color = ColorToVector4(GOLD);
	ACTOR_REGISTER_PRELOADASSETS(woodtorch);
	ACTOR_REGISTER_CLEANUP(woodtorch);
	ACTOR_REGISTER_DRAWWORLD(woodtorch);

	// Spawn child fire object
	ACTOR_FACTORY(act_fire, actor, Vector3RotateByQuaternion(Vector3Add(actor->position, Vector3Scale(VEC3UP,3.0f)), actor->rotation), QuaternionIdentity(), Vector3One(), Vector3Zero());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(woodtorch)
{
	// Load model
	Asset* model_asset = LoadAsset_Model(WOODTORCH_MODEL, FALSE);
	LoadAsset_Material(WOODTORCH_MATERIAL, FALSE);

	// Set collision data
	CollisionRegister(actor, &model_asset->mdl->meshes[WOODTORCH_MESH_COLLISION], COL_LAYER_WORLD);
}

ACTOR_CLEANUP(woodtorch)
{
	// clear collision data
	CollisionResign(actor, &AssetGet_Model(WOODTORCH_MODEL)->meshes[WOODTORCH_MESH_COLLISION]);
}

ACTOR_DRAWWORLD(woodtorch)
{
	if (OutOfRenderRange(actor))
		return;
	Material* mat = AssetGet_Material(WOODTORCH_MATERIAL);
	shader_update_fog(mat->shader);
	shader_update_lights(mat->shader);

	DrawMesh(
		AssetGet_Model(WOODTORCH_MODEL)->meshes[WOODTORCH_MESH_MAIN],
		*mat,
		GetMatrix(actor)
	);
}