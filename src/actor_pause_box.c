#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "input.h"
#include "game_state.h"
#include "camera.h"
#include "core_assets.h"

// Assets
#define PAUSEBOX_MODEL				ASSET_MODELS"/Tools/pausebox.glb"

// private header
ACTOR_PRELOADASSETS(pause_box);
ACTOR_UPDATE(pause_box);
ACTOR_ANIMATION_END(pause_box);
ACTOR_POSTDRAWWORLD(pause_box);
ACTOR_CLEANUP(pause_box);

typedef struct {
	int pause_time;
} PauseData;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(pause_box)
{
	// Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_PAUSED | ACTOR_FLAG_HAS_ANIMATIONS;
	ACTOR_REGISTER_PRELOADASSETS(pause_box);
	ACTOR_REGISTER_UPDATE(pause_box);
	ACTOR_REGISTER_ANIMATION_END(pause_box);
	ACTOR_REGISTER_POSTDRAWWORLD(pause_box);
	ACTOR_REGISTER_CLEANUP(pause_box);

	// Set data
	MALLOC_ACTOR_DATA(PauseData, actor->data);
	PauseData* pause_data = (PauseData*)actor->data;
	pause_data->pause_time = 0;

	// Pause the game
	printf("PAUSE\n");
	EXIT_GAMESTATE(GAMESTATE_GAMEPLAY);
	ENTER_GAMESTATE(GAMESTATE_PAUSED);

	struct Actor* camera = FINDACTOR_BYTYPE(act_camera);
	if (camera)
	{
		CameraData* cam_data = (CameraData*)camera->data;
		cam_data->locked = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(pause_box)
{
	LoadAsset_Material(ASSET_MATERIALS"/Engine/skinned_example.mat", FALSE);
	Asset* model_asset = LoadAsset_Model(PAUSEBOX_MODEL, FALSE);
	AddAnimLayer(actor, GetAnimation(model_asset, "HoldOpen"), ANIMATION_FRAMERATE, ANIM_LOOP, TRUE, 1.0f, BLENDTYPE_REPLACE);
	AddAnimLayer(actor, GetAnimation(model_asset, "HoldClosed"), ANIMATION_FRAMERATE, ANIM_LOOP, FALSE, 1.0f, BLENDTYPE_REPLACE);
	AddAnimLayer(actor, GetAnimation(model_asset, "CloseMenu"), ANIMATION_FRAMERATE, ANIM_SINGLE, FALSE, 1.0f, BLENDTYPE_REPLACE);
	AddAnimLayer(actor, GetAnimation(model_asset, "OpenMenu"), ANIMATION_FRAMERATE, ANIM_SINGLE, TRUE, 1.0f, BLENDTYPE_REPLACE);
}

ACTOR_UPDATE(pause_box)
{
	// Don't allow unpausing during the animation
	struct AnimationLayer* opening_layer = FindAnimLayer(actor, "OpenMenu");
	struct AnimationLayer* closing_layer = FindAnimLayer(actor, "CloseMenu");
	if (opening_layer->is_playing || closing_layer->is_playing)
		return;

	// Check if unpausing
	if (CHECK_INPUTPRESSED(input_pause))
	{
		printf("UNPAUSE\n");
		struct AnimationLayer* hold_close = FindAnimLayer(actor, "HoldClosed");
		struct AnimationLayer* hold_open = FindAnimLayer(actor, "HoldOpen");
		hold_close->is_playing = TRUE;
		hold_open->is_playing = FALSE;
		opening_layer->is_playing = FALSE;
		closing_layer->is_playing = TRUE; // Start closing animation
		return;
	}

	// Handle inventory
}

ACTOR_ANIMATION_END(pause_box)
{
	if(STRMATCH(animation, "CloseMenu"))
		ACTOR_DESTROY(actor);
}

ACTOR_POSTDRAWWORLD(pause_box)
{
	Material* mat = AssetGet_Material(ASSET_MATERIALS"/Engine/skinned_example.mat");
	Matrix box_mat = MatrixCompose(cam_main.position, QuaternionLookAt(cam_main.position, cam_main.target, VEC3UP), (Vector3) { 1.0f, 0.92f, 1.0f});
	
	Model* mdl = AssetGet_Model(PAUSEBOX_MODEL);
	ApplyAnimLayers(actor, mdl, mat, tick_percent);

	DrawMesh( // Forward
		mdl->meshes[3],
		*mat,
		box_mat
	);
	DrawMesh( // Right
		mdl->meshes[0],
		*mat,
		box_mat
	);
	DrawMesh( // Back
		mdl->meshes[2],
		*mat,
		box_mat
	);
	DrawMesh( // Left
		mdl->meshes[1],
		*mat,
		box_mat
	);
}

ACTOR_CLEANUP(pause_box)
{
	// Unpause the game
	EXIT_GAMESTATE(GAMESTATE_PAUSED);
	ENTER_GAMESTATE(GAMESTATE_GAMEPLAY);

	struct Actor* camera = FINDACTOR_BYTYPE(act_camera);
	if (camera)
	{
		CameraData* cam_data = (CameraData*)camera->data;
		cam_data->locked = FALSE;
	}
}