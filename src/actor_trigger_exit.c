#include "tools.h"
#include "assets.h"
#include "actor_trigger_exit.h"
#include "actor_factory.h"
#include "scene_entry.h"
#include "gamestate.h"
#include "player.h"
#include "game_draw.h"
#include "actor_fadein.h"

// Utility
#define MAX_FADEOUT_RATE 6
#define MAX_FADEOUT_TIME 300

// private header
ACTOR_PREUPDATE(trigger_exit);
ACTOR_DRAWWORLD(trigger_exit);
ACTOR_POSTDRAWHUD(trigger_exit);
static void actor_exit_startleaving(struct Actor* exit, struct Actor* player);
static void actor_exit_finishleaving(struct Actor* exit);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(trigger_exit)
{
	// Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_TRANSITION;
	ACTOR_REGISTER_PREUPDATE(trigger_exit);
	ACTOR_REGISTER_DRAWWORLD(trigger_exit);
	ACTOR_REGISTER_POSTDRAWHUD(trigger_exit);

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

ACTOR_PREUPDATE(trigger_exit)
{
	struct Actor* player = FINDACTORTYPE(act_player);
	TriggerExitData* exit_data = actor->data;
	if (exit_data->is_triggered)
	{
		// Wait for fadeout before entering the new scene
		exit_data->previous_fadeout = exit_data->fadeout;
		exit_data->fadeout += MAX_FADEOUT_RATE;
		if (exit_data->fadeout >= MAX_FADEOUT_TIME)
		{
			exit_data->fadeout = MAX_FADEOUT_TIME;
			actor_exit_finishleaving(actor);
		}
	}
	else
	{
		// Wait for player to enter trigger
		if (!player)
			return;
		if (Vector3Distance(actor->position, player->position) < exit_data->radius)
			actor_exit_startleaving(actor, player);
	}
}

ACTOR_DRAWWORLD(trigger_exit)
{
	if (!draw_debug_info)
		return;
	TriggerExitData* exit_data = actor->data;
	DrawSphereWires(actor->position, exit_data->radius, 10, 10, WHITE);
}

ACTOR_POSTDRAWHUD(trigger_exit)
{
	TriggerExitData* exit_data = actor->data;
	DrawRectangle(0, 0, renderWidth, renderHeight, (Color) { 0, 0, 0, (int)Clamp( Lerp((float)exit_data->previous_fadeout, (float)exit_data->fadeout, (float)tick_percent),0,255) });
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
		CameraSetMode(camera, CAMERA_MODE_ONLYWATCH);
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
