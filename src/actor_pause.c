#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "input.h"
#include "gamestate.h"
#include "camera.h"

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
	printf("time %i\n", pause_data->pause_time);
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
	DrawCube(Vector3Add(Vector3Scale(VEC3FORWARD, 5.0f), Vector3Scale(ACTOR_POS_DELTA(actor, (float)tick_percent), 0.01f)), 0.2f, 0.2f, 0.2f, RED);
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