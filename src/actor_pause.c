#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "input.h"
#include "gamestate.h"
#include "camera.h"
#include "core_assets.h"

// private header
static void actor_pause_preload_assets(struct Actor* actor);
static void actor_pause_update(struct Actor* actor);
static void actor_pause_postdrawworld(struct Actor* actor, double delta_time);
static void actor_pause_destroy(struct Actor* actor);
static void actor_pause_animation_ended(struct Actor* actor, char* animation);

typedef struct {
	int pause_time;
} PauseData;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void actor_pause_init(struct Actor* actor)
{
	// Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_PAUSED | ACTOR_FLAG_HAS_ANIMATIONS;
	actor->func_preloadassets = actor_pause_preload_assets;
	actor->func_update = actor_pause_update;
	actor->func_postdrawworld = actor_pause_postdrawworld;
	actor->func_destroy = actor_pause_destroy;
	actor->func_animation_ended = actor_pause_animation_ended;

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

static void actor_pause_preload_assets(struct Actor* actor)
{
	Asset* model_asset = AssetGetPackage(PAUSEBOX_MODEL);
	AddAnimLayer(actor, GetAnimation(model_asset, "HoldOpen"), ANIMATION_FRAMERATE, ANIM_LOOP, TRUE, 1.0f, BLENDTYPE_REPLACE);
	AddAnimLayer(actor, GetAnimation(model_asset, "HoldClosed"), ANIMATION_FRAMERATE, ANIM_LOOP, FALSE, 1.0f, BLENDTYPE_REPLACE);
	AddAnimLayer(actor, GetAnimation(model_asset, "CloseMenu"), ANIMATION_FRAMERATE, ANIM_SINGLE, FALSE, 1.0f, BLENDTYPE_REPLACE);
	AddAnimLayer(actor, GetAnimation(model_asset, "OpenMenu"), ANIMATION_FRAMERATE, ANIM_SINGLE, TRUE, 1.0f, BLENDTYPE_REPLACE);
}

static void actor_pause_update(struct Actor* actor)
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

static void actor_pause_animation_ended(struct Actor* actor, char* animation)
{
	if(STRMATCH(animation, "CloseMenu"))
		ACTOR_DESTROY(actor);
}

static void actor_pause_postdrawworld(struct Actor* actor, double tick_percent)
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

static void actor_pause_destroy(struct Actor* actor)
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