#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "input.h"
#include "gamestate.h"
#include "camera.h"
#include "core_assets.h"

// Assets
#define PAUSEBOX_MODEL				ASSET_MODELS"/Tools/pausebox.glb"

// private header
ACTOR_PRELOADASSETS(pause);
ACTOR_UPDATE(pause);
ACTOR_ANIMATION_END(pause);
ACTOR_POSTDRAWWORLD(pause);
ACTOR_CLEANUP(pause);

typedef struct {
	int pause_time;
} PauseData;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(pause)
{
	// Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_PAUSED | ACTOR_FLAG_HAS_ANIMATIONS;
	ACTOR_REGISTER_PRELOADASSETS(pause);
	ACTOR_REGISTER_UPDATE(pause);
	ACTOR_REGISTER_ANIMATION_END(pause);
	ACTOR_REGISTER_POSTDRAWWORLD(pause);
	ACTOR_REGISTER_CLEANUP(pause);

	// Set data
	MALLOC_ACTOR_DATA(PauseData, actor->data);
	PauseData* pause_data = (PauseData*)actor->data;
	pause_data->pause_time = 0;

	// Pause the game
	printf("PAUSE\n");
	gameplay_state &= ~GAMESTATE_GAMEPLAY;
	gameplay_state |= GAMESTATE_PAUSED;

	struct Actor* camera = FINDACTORTYPE(act_camera);
	if (camera)
	{
		CameraData* cam_data = (CameraData*)camera->data;
		cam_data->locked = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(pause)
{
	Asset* model_asset = LoadAsset_Model(PAUSEBOX_MODEL, FALSE);
	AddAnimLayer(actor, GetAnimation(model_asset, "HoldOpen"), ANIMATION_FRAMERATE, ANIM_LOOP, TRUE, 1.0f, BLENDTYPE_REPLACE);
	AddAnimLayer(actor, GetAnimation(model_asset, "HoldClosed"), ANIMATION_FRAMERATE, ANIM_LOOP, FALSE, 1.0f, BLENDTYPE_REPLACE);
	AddAnimLayer(actor, GetAnimation(model_asset, "CloseMenu"), ANIMATION_FRAMERATE, ANIM_SINGLE, FALSE, 1.0f, BLENDTYPE_REPLACE);
	AddAnimLayer(actor, GetAnimation(model_asset, "OpenMenu"), ANIMATION_FRAMERATE, ANIM_SINGLE, TRUE, 1.0f, BLENDTYPE_REPLACE);
}

ACTOR_UPDATE(pause)
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

ACTOR_ANIMATION_END(pause)
{
	if(STRMATCH(animation, "CloseMenu"))
		ACTOR_DESTROY(actor);
}

ACTOR_POSTDRAWWORLD(pause)
{
	Material* mat = AssetGet_Material(ASSET_MATERIALS"/Error/no_material.mat");
	Material* mat_f = AssetGet_Material(ASSET_MATERIALS"/Objects/example.mat");
	Matrix box_mat = MatrixCompose(cam_main.position, QuaternionLookAt(cam_main.position, cam_main.target, VEC3UP), (Vector3) { 1.0f, 0.92f, 1.0f});
	
	Model* mdl = AssetGet_Model(PAUSEBOX_MODEL);
	ApplyAnimLayers(actor, mdl, tick_percent);

	DrawMesh( // Forward
		mdl->meshes[3],
		*mat_f,
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

ACTOR_CLEANUP(pause)
{
	// Unpause the game
	gameplay_state &= ~GAMESTATE_PAUSED;
	gameplay_state |= GAMESTATE_GAMEPLAY;

	struct Actor* camera = FINDACTORTYPE(act_camera);
	if (camera)
	{
		CameraData* cam_data = (CameraData*)camera->data;
		cam_data->locked = FALSE;
	}
}