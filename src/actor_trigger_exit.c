#include "tools.h"
#include "assets.h"
#include "actor_trigger_exit.h"
#include "actor_factory.h"
#include "scene_entry.h"
#include "game_state.h"
#include "player.h"
#include "game_draw.h"
#include "actor_fade.h"

// private header
ACTOR_JSON_INIT(trigger_exit);
ACTOR_PREUPDATE(trigger_exit);
ACTOR_DRAWWORLD(trigger_exit);
static void actor_exit_startleaving(struct Actor* exit, struct Actor* player);
static void actor_exit_finishleaving(struct Actor* exit);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(trigger_exit)
{
	// Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_TRANSITION;
	ACTOR_REGISTER_JSON_INIT(trigger_exit);
	ACTOR_REGISTER_PREUPDATE(trigger_exit);
	ACTOR_REGISTER_DRAWWORLD(trigger_exit);

	// Set data
	MALLOC_ACTOR_DATA(TriggerExitData, actor->data);
}

ACTOR_JSON_INIT(trigger_exit)
{
	if (file_data == NULL)
		return;

	TriggerExitData* exit_data = actor->data;
	exit_data->dest_scene = SCENE_FROM_STRING(cJSON_GetObjectItem(file_data, "to_scene")->valuestring);
	exit_data->dest_entrance = ENTRANCE_FROM_STRING(cJSON_GetObjectItem(file_data, "to_entrance")->valuestring);
	exit_data->radius = (float)cJSON_GetObjectItem(file_data, "radius")->valuedouble;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PREUPDATE(trigger_exit)
{
	TriggerExitData* exit_data = actor->data;
	struct Actor* fade_actor = FINDACTORTYPE(act_fadeout);
	if (fade_actor)
	{
		// Wait for fadeout before entering the new scene
		FadeInData* fade_data = fade_actor->data;
		if (fade_data->fadeout >= 255)
		{
			ACTOR_DESTROY(fade_actor);
			actor_exit_finishleaving(actor);
			actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
		}
		return;
	}
	// Wait for player to enter trigger
	struct Actor* player = FINDACTORTYPE(act_player);
	if (!player)
		return;
	if (Vector3Distance(actor->position, player->position) > exit_data->radius)
		return;
	actor_exit_startleaving(actor, player);
}

ACTOR_DRAWWORLD(trigger_exit)
{
	if (!draw_debug_info)
		return;
	TriggerExitData* exit_data = actor->data;
	DrawSphereWires(actor->position, exit_data->radius, 10, 10, WHITE);
}

static void actor_exit_startleaving(struct Actor* exit, struct Actor* player)
{
	// Set the game into transition state
	TriggerExitData* exit_data = exit->data;
	gameplay_state &= ~GAMESTATE_GAMEPLAY;
	gameplay_state |= GAMESTATE_TRANSITION;
	printf("EXIT TRIGGERED\n");
	FADEOUT_CREATE(BLACK);

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
