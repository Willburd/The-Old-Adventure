#include "core_assets.h"
#include "collision.h"
#include "actor_factory.h"
#include "tools.h"
#include "actor_textbox.h"
#include "game_draw.h"
#include "camera.h"

// Assets
#define SIGN_MODEL ASSET_MODELS"/Objects/wood_sign.glb"
#define SIGN_MATERIAL ASSET_MATERIALS"/Objects/wood_sign_a.mat"

// private header
ACTOR_PRELOADASSETS(signpost);
ACTOR_INTERACT_TEXT(signpost);
ACTOR_CAN_INTERACT(signpost);
ACTOR_PLAYER_INTERACT(signpost);
ACTOR_DRAWWORLD(signpost);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(signpost)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_INTERACTIVE;
	ACTOR_REGISTER_PRELOADASSETS(signpost);
	ACTOR_REGISTER_DRAWWORLD(signpost);
	ACTOR_REGISTER_CAN_INTERACT(signpost);
	ACTOR_REGISTER_PLAYER_INTERACT(signpost);
	ACTOR_REGISTER_INTERACT_TEXT(signpost);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(signpost)
{
	// Load model
	Asset* model_asset = LoadAsset_Model(SIGN_MODEL, FALSE);
	LoadAsset_Material(SIGN_MATERIAL, FALSE);

	// Set collision data
	REGISTER_COLLISION_MESH(actor, model_asset, DEFAULT_COLLISION_MESH, COL_LAYER_WORLD);
}

ACTOR_CAN_INTERACT(signpost)
{
	if (OutOfRenderRange(actor))
		return FALSE;
	Vector3 our_forwardvec = Vector3RotateByQuaternion(VEC3FORWARD, actor->rotation);
	Vector3 ply_forwardvec = Vector3RotateByQuaternion(VEC3FORWARD, player->rotation);
	return Vector3DotProduct(our_forwardvec, ply_forwardvec) < -0.2; // Facing off against each other
}

ACTOR_INTERACT_TEXT(signpost)
{
	return TEXT_ACTIONBUTTON_READ;
}

ACTOR_PLAYER_INTERACT(signpost)
{
	// Start camera focus
	struct Actor* camera = FINDACTORTYPE(act_camera);
	CameraSetMode(camera, CAMERA_MODE_FOCUS_CUTSCENE_SLOW);
	SetCutsceneCameraLookPos(camera, Vector3Add(actor->position, VEC3UP));
	// Textbox display
	TEXTBOX_CREATE(actor, player, TEXT_TEST_DEBUG, TEXTBOX_DEFAULT_SPEED);
}

ACTOR_DRAWWORLD(signpost)
{
	if (OutOfRenderRange(actor))
		return;
	Asset* model_asset = LoadAsset_Model(SIGN_MODEL, FALSE);

	STANDARD_SHADER_MATERIAL(sign_mat, SIGN_MATERIAL, actor);
	ToaDrawMesh(
		model_asset,
		GetMeshIndex(model_asset->mesh_data, "Sign-Main"),
		*sign_mat,
		GetMatrix(actor),
		FALSE
	);
}