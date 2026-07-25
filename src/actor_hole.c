#include "actor_factory.h"
#include "tools.h"
#include "camera.h"
#include "game_state.h"
#include "actor_trigger_exit.h"
#include "actor_fadein.h"
#include "actor_scene.h"
#include "player.h"
#include "core_assets.h"
#include "game_draw.h"

// Assets
#define HOLE_MATERIAL_MAIN ASSET_MATERIALS"/Effects/pit_warp.mat"

// Utility
#define MAX_FADEOUT_RATE 6
#define MAX_FADEOUT_TIME 300

// private header
ACTOR_PRELOADASSETS(hole);
ACTOR_JSON_INIT(hole);
ACTOR_UPDATE(hole);
ACTOR_TRANSPARENTDRAWWORLD(hole);
ACTOR_POSTDRAWHUD(hole);
static void actor_hole_startleaving(struct Actor* exit, struct Actor* player);
static void actor_hole_finishleaving(struct Actor* exit);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(hole)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | GAMESTATE_TRANSITION;
	ACTOR_REGISTER_PRELOADASSETS(hole);
	ACTOR_REGISTER_JSON_INIT(hole);
	ACTOR_REGISTER_UPDATE(hole);
	ACTOR_REGISTER_TRANSPARENTDRAWWORLD(hole);
	ACTOR_REGISTER_POSTDRAWHUD(hole);

	// Set data
	MALLOC_ACTOR_DATA(TriggerExitData, actor->data);
}

ACTOR_JSON_INIT(hole)
{
	if (file_data == NULL)
		return;

	TriggerExitData* exit_data = actor->data;
	exit_data->dest_scene = SCENE_FROM_STRING(cJSON_GetObjectItem(file_data, "to_scene")->valuestring);
	exit_data->dest_entrance = ENTRANCE_FROM_STRING(cJSON_GetObjectItem(file_data, "to_entrance")->valuestring);
	exit_data->radius = 1.0f;
	exit_data->is_triggered = FALSE;
	exit_data->previous_fadeout = 0;
	exit_data->fadeout = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(hole)
{
	LoadAsset_Material(HOLE_MATERIAL_MAIN, FALSE);
}

ACTOR_UPDATE(hole)
{
	TriggerExitData* exit_data = actor->data;
	if (exit_data->is_triggered)
	{
		// Wait for fadeout before entering the new scene
		exit_data->previous_fadeout = exit_data->fadeout;
		exit_data->fadeout += MAX_FADEOUT_RATE;
		if (exit_data->fadeout >= MAX_FADEOUT_TIME)
		{
			exit_data->fadeout = MAX_FADEOUT_TIME;
			actor_hole_finishleaving(actor);
		}
		return;
	}
	// Wait for player to enter trigger
	struct Actor* player = FINDACTORTYPE(act_player);
	if (!player)
		return;
	if (Vector3Distance(actor->position, player->position) > exit_data->radius)
		return;
	actor_hole_startleaving(actor, player);
}

ACTOR_TRANSPARENTDRAWWORLD(hole)
{
	if (OutOfRenderRange(actor))
		return;
	STANDARD_SHADER_MATERIAL(hole_mat, HOLE_MATERIAL_MAIN, actor);
	Transform hole_transform = {
		.translation = Vector3Add(actor->position, (Vector3) { 0.0f, 0.1f, 0.0f }),
		.rotation = QuaternionMultiply(QuaternionFlatLookAt(cam_main.position, actor->position, VEC3UP), QuaternionFromEuler(270.0f * DEG2RAD, 0.0f, 0.0f)),
		.scale = Vector3Multiply(actor->scale, (Vector3) { 2.0f, 2.0f, 2.0f })
	};

	DrawMesh(
		AssetGet_Model(SPRITE_MODEL)->meshes[0],
		*hole_mat,
		MATRIX_ASSEMBLE(hole_transform)
	);
}

ACTOR_POSTDRAWHUD(hole)
{
	TriggerExitData* exit_data = actor->data;
	DrawRectangle(0, 0, renderWidth, renderHeight, (Color) { 0, 0, 0, (int)Clamp(Lerp((float)exit_data->previous_fadeout, (float)exit_data->fadeout, (float)tick_percent), 0, 255) });
}

static void actor_hole_startleaving(struct Actor* exit, struct Actor* player)
{
	// Set the game into transition state
	TriggerExitData* exit_data = exit->data;
	exit_data->is_triggered = TRUE;
	gameplay_state &= ~GAMESTATE_GAMEPLAY;
	gameplay_state |= GAMESTATE_TRANSITION;
	printf("HOLE TRIGGERED\n");

	// Make player fall out of the world
	PlayerData* player_data = (PlayerData*)player->data;
	player_data->disable_collision = TRUE;
	float original_y_vel = player->velocity.y;
	player->velocity = Vector3Scale(Vector3FlatDirection(player->position, exit->position), 0.46f); // TOWARD the hole
	player->velocity.y = original_y_vel - 0.2f;

	// Lock the camera in place
	struct Actor* camera = FINDACTORTYPE(act_camera);
	if (camera)
		CameraSetMode(camera, CAMERA_MODE_ONLYWATCH);
}

static void actor_hole_finishleaving(struct Actor* exit)
{
	// Finish transitioning to the newscene
	TriggerExitData* exit_data = exit->data;
	gameplay_state &= ~GAMESTATE_TRANSITION;
	gameplay_state |= GAMESTATE_GAMEPLAY;
	printf("HOLE FINISHED\n");

	// Create transition fade into new room, it's not tied to any scenes, so it fades out on it's own
	FADEIN_CREATE(BLACK);
	TransferScene(exit_data->dest_scene, exit_data->dest_entrance);
}