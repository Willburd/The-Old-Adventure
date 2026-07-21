#include "actor_factory.h"
#include "tools.h"
#include "collision.h"
#include "game_draw.h"
#include "models.h"

// Assets
#define WOODTORCH_MODEL ASSET_MODELS"/Objects/wood_torch.glb"
#define WOODTORCH_MATERIAL ASSET_MATERIALS"/Objects/wood_door_a.mat"

// private header
ACTOR_PRELOADASSETS(woodtorch);
ACTOR_DRAWWORLD(woodtorch);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(woodtorch)
{
	actor->actor_flags = 0;
	actor->blend_color = ColorToVector4(GOLD);
	ACTOR_REGISTER_PRELOADASSETS(woodtorch);
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
	REGISTER_COLLISION_MESH(actor, model_asset, DEFAULT_COLLISION_MESH, COL_LAYER_WORLD);
}

ACTOR_DRAWWORLD(woodtorch)
{
	if (OutOfRenderRange(actor))
		return;
	Asset* model_asset = AssetGetPackage(WOODTORCH_MODEL);
	STANDARD_SHADER_MATERIAL(mat, WOODTORCH_MATERIAL, actor);

	int main_mesh_index = GetMeshIndex(model_asset->mesh_data, "Torch-Torch");
	ToaDrawMesh(
		model_asset,
		main_mesh_index,
		*mat,
		GetMatrix(actor),
		TRUE
	);
}