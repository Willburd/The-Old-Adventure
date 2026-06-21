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
	actor->actor_flags = ACTOR_FLAG_TICKDURING_PAUSED; // | ACTOR_FLAG_HAS_ANIMATIONS;
	actor->func_preloadassets = actor_pause_preload_assets;
	actor->func_update = actor_pause_update;
	actor->func_postdrawworld = actor_pause_postdrawworld;
	actor->func_destroy = actor_pause_destroy;

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

}

static void actor_pause_update(struct Actor* actor)
{
	// Don't allow unpausing during the animation
	PauseData* pause_data = (PauseData*)actor->data;
	pause_data->pause_time += 1;
	if (pause_data->pause_time < 20)
		return;

	// Check if unpausing
	if (CHECK_INPUTPRESSED(input_pause))
	{
		printf("UNPAUSE\n");
		ACTOR_DESTROY(actor);
		return;
	}

	// Handle inventory
}

static void actor_pause_postdrawworld(struct Actor* actor, double tick_percent)
{
	Material* mat = AssetGet_Material(ASSET_MATERIALS"/Error/no_material.mat");
	Material* mat_f = AssetGet_Material(ASSET_MATERIALS"/Objects/example.mat");
	Matrix box_mat = MatrixCompose(cam_main.position, QuaternionLookAt(cam_main.position, cam_main.target, VEC3UP), (Vector3) { 1.0f, 0.92f, 1.0f});

	DrawMesh( // Forward
		AssetGet_Model(PAUSEBOX_MODEL)->meshes[2],
		*mat_f,
		box_mat
	);
	DrawMesh( // Right
		AssetGet_Model(PAUSEBOX_MODEL)->meshes[3],
		*mat,
		box_mat
	);
	DrawMesh( // Back
		AssetGet_Model(PAUSEBOX_MODEL)->meshes[0],
		*mat,
		box_mat
	);
	DrawMesh( // Left
		AssetGet_Model(PAUSEBOX_MODEL)->meshes[1],
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