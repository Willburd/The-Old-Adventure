#include "actor_factory.h"
#include "tools.h"
#include "collision.h"
#include "game_draw.h"

// Assets
#define WOODTORCH_ASSET_MAIN_MODEL ASSET_MODELS"/Objects/wood_torch.glb"
#define WOODTORCH_ASSET_MAIN_MATERIAL ASSET_MATERIALS"/Objects/wood_torch.mat"

// Utility
#define MAIN_MODEL_MESH_MAIN 0
#define MAIN_MODEL_MESH_COLLISION 1

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
	Asset* model_asset = LoadAsset_Model(WOODTORCH_ASSET_MAIN_MODEL, FALSE);
	LoadAsset_Material(WOODTORCH_ASSET_MAIN_MATERIAL, FALSE);

	// Set collision data
	actor->collision_flags = COL_LAYER_WORLD;
	CollisionRegister(actor, &model_asset->mdl->meshes[MAIN_MODEL_MESH_COLLISION]);
}

ACTOR_CLEANUP(woodtorch)
{
	// clear collision data
	CollisionResign(actor, &AssetGet_Model(WOODTORCH_ASSET_MAIN_MODEL)->meshes[MAIN_MODEL_MESH_COLLISION]);
}

ACTOR_DRAWWORLD(woodtorch)
{
	if (OutOfRenderRange(actor))
		return;
	Material* mat = AssetGet_Material(WOODTORCH_ASSET_MAIN_MATERIAL);
	shader_update_fog(mat->shader);
	shader_update_lights(mat->shader);

	DrawMesh(
		AssetGet_Model(WOODTORCH_ASSET_MAIN_MODEL)->meshes[MAIN_MODEL_MESH_MAIN],
		*mat,
		GetMatrix(actor)
	);
}