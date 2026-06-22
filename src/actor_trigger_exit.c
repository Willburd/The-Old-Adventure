#include "tools.h"
#include "assets.h"
#include "actor_trigger_exit.h"
#include "actor_factory.h"
#include "scene_entry.h"
#include "gamestate.h"
#include "player.h"
#include "game_draw.h"
#include "actor_fadein.h"

// private header
static void actor_trigger_exit_preupdate(struct Actor* actor);
static void actor_trigger_exit_drawworld(struct Actor* actor, double tick_percent);
static void actor_trigger_exit_postdrawhud(struct Actor* exit, double tick_percent);
static void actor_exit_startleaving(struct Actor* exit, struct Actor* player);
static void actor_exit_finishleaving(struct Actor* exit);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void actor_trigger_exit_init(struct Actor* actor)
{
	// Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_TRANSITION;
	actor->func_preupdate = actor_trigger_exit_preupdate;
	actor->func_drawworld = actor_trigger_exit_drawworld;
	actor->func_postdrawhud = actor_trigger_exit_postdrawhud;

	// Set data
	MALLOC_ACTOR_DATA(TriggerExitData, actor->data);
}

struct Actor* EXIT_TRIGGER_CREATE(int destination_scene, int destination_entrance, struct Actor* scene, Vector3 pos, float radius)
{
	struct Actor* exit = ACTOR_FACTORY(act_trigger_exit, scene, pos, QuaternionIdentity(), Vector3One(), Vector3Zero());
	TriggerExitData* exit_data = exit->data;
	exit_data->dest_scene = destination_scene;
	exit_data->dest_entrance = destination_entrance;
	exit_data->radius = radius;
	exit_data->is_triggered = FALSE;
	exit_data->previous_fadeout = 0;
	exit_data->fadeout = 0;
	return exit;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_FADEOUT_RATE 6
#define MAX_FADEOUT_TIME 300

static void actor_trigger_exit_preupdate(struct Actor* exit)
{
	struct Actor* player = FINDACTORTYPE(act_player);
	TriggerExitData* exit_data = exit->data;
	if (exit_data->is_triggered)
	{
		// Wait for fadeout before entering the new scene
		exit_data->previous_fadeout = exit_data->fadeout;
		exit_data->fadeout += MAX_FADEOUT_RATE;
		if (exit_data->fadeout >= MAX_FADEOUT_TIME)
		{
			exit_data->fadeout = MAX_FADEOUT_TIME;
			actor_exit_finishleaving(exit);
		}
	}
	else
	{
		// Wait for player to enter trigger
		if (!player)
			return;
		if (Vector3Distance(exit->position, player->position) < exit_data->radius)
			actor_exit_startleaving(exit, player);
	}
}

static void actor_trigger_exit_drawworld(struct Actor* exit, double tick_percent)
{
	if (!draw_debug_info)
		return;
	TriggerExitData* exit_data = exit->data;
	DrawSphereWires(exit->position, exit_data->radius, 10, 10, WHITE);
}

static void actor_trigger_exit_postdrawhud(struct Actor* exit, double tick_percent)
{
	TriggerExitData* exit_data = exit->data;
	DrawRectangle(0, 0, renderWidth, renderHeight, (Color) { 0, 0, 0, Clamp( Lerp(exit_data->previous_fadeout, exit_data->fadeout, tick_percent),0,255) });
}

static void actor_exit_startleaving(struct Actor* exit, struct Actor* player)
{
	// Set the game into transition state
	TriggerExitData* exit_data = exit->data;
	exit_data->is_triggered = TRUE;
	gameplay_state &= ~GAMESTATE_GAMEPLAY;
	gameplay_state |= GAMESTATE_TRANSITION;
	printf("EXIT TRIGGERED\n");

	// Run to the entrance
	PlayerData* player_data = (PlayerData*)player->data;
	player_data->cutscene_run_goal = exit->position;
	player_data->cutscene_run_factor = 1.0f;

	// Lock the camera in place
	struct Actor* camera = FINDACTORTYPE(act_camera);
	if (camera)
	{
		CameraData* cam_data = (CameraData*)camera->data;
		cam_data->camera_mode = CAMERA_MODE_ONLYWATCH;
	}
}

static void actor_exit_finishleaving(struct Actor* exit)
{
	// Finish transitioning to the newscene
	TriggerExitData* exit_data = exit->data;
	gameplay_state &= ~GAMESTATE_TRANSITION;
	gameplay_state |= GAMESTATE_GAMEPLAY;
	printf("EXIT FINISHED\n");

	// Create transition fade into new room, it's not tied to any scenes, so it fades out on it's own
	FADEIN_CREATE(BLACK);
	TransferScene(exit_data->dest_scene, exit_data->dest_entrance);
}
