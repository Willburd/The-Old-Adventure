#include "../actor_factory.h"
#include "../tools.h"
#include "../camera.h"
#include "../game_state.h"
#include "actor_trigger_exit.h"
#include "../actor_fade.h"
#include "../actor_scene.h"
#include "player.h"
#include "core_assets.h"
#include "../game_draw.h"
#include "json_properties.h"

// Assets
#define HOLE_MATERIAL_MAIN ASSET_MATERIALS"/Effects/pit_warp.mat"

// private header
ACTOR_PRELOADASSETS(hole);
ACTOR_JSON_INIT(hole);
ACTOR_UPDATE(hole);
ACTOR_TRANSPARENTDRAWWORLD(hole);
static void actor_hole_startleaving(struct Actor* exit, struct Actor* player);
static void actor_hole_finishleaving(struct Actor* exit);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(hole)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME;
	ACTOR_REGISTER_PRELOADASSETS(hole);
	ACTOR_REGISTER_JSON_INIT(hole);
	ACTOR_REGISTER_UPDATE(hole);
	ACTOR_REGISTER_TRANSPARENTDRAWWORLD(hole);

	// Set data
	MALLOC_ACTOR_DATA(TriggerExitData, actor->data);
}

ACTOR_JSON_INIT(hole)
{
	if (file_data == NULL)
		return;

	TriggerExitData* exit_data = actor->data;
	exit_data->dest_scene = SCENE_FROM_STRING(cJSON_GetObjectItem(file_data, PROP_TO_SCENE)->valuestring);
	exit_data->dest_entrance = ENTRANCE_FROM_STRING(cJSON_GetObjectItem(file_data, PROP_TO_ENTRANCE)->valuestring);
	exit_data->radius = 1.0f;
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
	struct Actor* fade_actor = FINDACTOR_BYTYPE(act_fadeout);
	if (fade_actor)
	{
		// Wait for fadeout before entering the new scene
		FadeInData* fade_data = fade_actor->data;
		if (fade_data->fadeout >= 255)
		{
			ACTOR_DESTROY(fade_actor);
			actor_hole_finishleaving(actor);
			actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
		}
		return;
	}
	// Wait for player to enter trigger
	struct Actor* player = FINDACTOR_BYTYPE(act_player);
	if (!player)
		return;
	if (abs(player->position.y - actor->position.y) > 0.1f)
		return;
	if (Vector3Distance(actor->position, player->position) > exit_data->radius)
		return;
	actor->actor_flags |= ACTOR_FLAG_TICKDURING_TRANSITION;
	actor_hole_startleaving(actor, player);
}

ACTOR_TRANSPARENTDRAWWORLD(hole)
{
	if (OutOfRenderRange(actor))
		return;
	STANDARD_SHADER_MATERIAL(hole_mat, HOLE_MATERIAL_MAIN, actor);
	Transform hole_transform = {
		.translation = Vector3Add(actor->position, (Vector3) { 0.0f, 0.1f, 0.0f }),
		.rotation = QuaternionMultiply(QuaternionFlatLookAt(actor->position, cam_main.position, VEC3UP), QuaternionFromEuler(90.0f * DEG2RAD, 0.0f, 0.0f)),
		.scale = Vector3Multiply(actor->scale, (Vector3) { 2.0f, 2.0f, 2.0f })
	};

	DrawMesh(
		AssetGet_Model(SPRITE_MODEL)->meshes[0],
		*hole_mat,
		MATRIX_ASSEMBLE(hole_transform)
	);
}

static void actor_hole_startleaving(struct Actor* exit, struct Actor* player)
{
	// Set the game into transition state
	TriggerExitData* exit_data = exit->data;
	EXIT_GAMESTATE(GAMESTATE_GAMEPLAY);
	ENTER_GAMESTATE(GAMESTATE_TRANSITION);
	printf("HOLE TRIGGERED\n");
	FADEOUT_CREATE(BLACK);

	// Make player fall out of the world
	PlayerData* player_data = (PlayerData*)player->data;
	player_data->disable_collision = TRUE;
	float original_y_vel = player->velocity.y;
	player->velocity = Vector3Scale(Vector3FlatDirection(player->position, exit->position), 0.46f); // TOWARD the hole
	player->velocity.y = original_y_vel - 0.2f;

	// Lock the camera in place
	struct Actor* camera = FINDACTOR_BYTYPE(act_camera);
	if (camera)
		CameraSetMode(camera, CAMERA_MODE_ONLYWATCH);
}

static void actor_hole_finishleaving(struct Actor* exit)
{
	// Finish transitioning to the newscene
	TriggerExitData* exit_data = exit->data;
	EXIT_GAMESTATE(GAMESTATE_TRANSITION);
	ENTER_GAMESTATE(GAMESTATE_GAMEPLAY);
	printf("HOLE FINISHED\n");

	// Create transition fade into new room, it's not tied to any scenes, so it fades out on it's own
	FADEIN_CREATE(BLACK);
	TransferScene(exit_data->dest_scene, exit_data->dest_entrance);
}