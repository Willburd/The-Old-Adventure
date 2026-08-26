#include "core_assets.h"
#include "collision.h"
#include "actor_factory.h"
#include "tools.h"
#include "actor_textbox.h"
#include "game_draw.h"
#include "camera.h"
#include "json_properties.h"
#include "text_loading.h"

// Assets
#define SIGN_MODEL ASSET_MODELS"/Objects/wood_sign.glb"
static const char* loaded_materials[] = {
	ASSET_MATERIALS"/Objects/wood_sign_a.mat" // Sign
};

// private header
ACTOR_PRELOADASSETS(signpost);
ACTOR_JSON_INIT(signpost);
ACTOR_INTERACT_TEXT(signpost);
ACTOR_CAN_INTERACT(signpost);
ACTOR_PLAYER_INTERACT(signpost);
ACTOR_DRAWWORLD(signpost);
ACTOR_CLEANUP(signpost);

typedef struct
{
	char* text_entry_id;
} SignData;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(signpost)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_INTERACTIVE;
	ACTOR_REGISTER_PRELOADASSETS(signpost);
	ACTOR_REGISTER_JSON_INIT(signpost);
	ACTOR_REGISTER_DRAWWORLD(signpost);
	ACTOR_REGISTER_CAN_INTERACT(signpost);
	ACTOR_REGISTER_PLAYER_INTERACT(signpost);
	ACTOR_REGISTER_INTERACT_TEXT(signpost);
	ACTOR_REGISTER_CLEANUP(signpost);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(signpost)
{
	// Load model
	LoadAsset_Model(SIGN_MODEL, FALSE);
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));

	// Set collision data
	REGISTER_COLLISION_MESH(actor, AssetGetPackage(SIMPLE_ACTOR_COLLISION_MODEL), DEFAULT_COLLISION_MESH, COL_LAYER_WORLD);

	// Set data
	MALLOC_ACTOR_DATA(SignData, actor->data);
	SignData* sign_data = actor->data;
	sign_data->text_entry_id = NULL;
}

ACTOR_JSON_INIT(signpost)
{
	if (file_data == NULL)
		return;

	SignData* sign_data = actor->data;
	sign_data->text_entry_id = TEXT_TEST_DEBUG;
	if (cJSON_GetObjectItem(file_data, PROP_TEXT_ID))
	{
		char* text_id_data = cJSON_GetObjectItem(file_data, PROP_TEXT_ID)->valuestring;
		CHAR_STR_COPY(sign_data->text_entry_id, text_id_data, NULL);
		LoadTextData(sign_data->text_entry_id);
	}
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
	struct Actor* camera = FINDACTOR_BYTYPE(act_camera);
	CameraSetMode(camera, CAMERA_MODE_FOCUS_CUTSCENE_SLOW);
	SetCutsceneCameraLookPos(camera, Vector3Add(actor->position, VEC3UP));
	// Textbox display
	SignData* sign_data = actor->data;
	TEXTBOX_CREATE(actor, player, sign_data->text_entry_id, TEXTBOX_DEFAULT_SPEED);
}

ACTOR_DRAWWORLD(signpost)
{
	if (OutOfRenderRange(actor))
		return;
	DrawAllModelMeshes(actor, SIGN_MODEL, loaded_materials);
}

ACTOR_CLEANUP(signpost)
{
	SignData* sign_data = actor->data;
	if (sign_data->text_entry_id != NULL)
	{
		char* ptr = sign_data->text_entry_id;
		RELEASE(ptr); // Allocated char string
	}
}