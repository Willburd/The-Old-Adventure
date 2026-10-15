#include "../tools.h"
#include "../assets.h"
#include "../actor_factory.h"
#include "../scene_entry.h"
#include "../game_draw.h"
#include "../collision.h"
#include "../game_state.h"
#include "actor_fade.h"
#include "actor_camera.h"
#include "player.h"
#include "inventory.h"

/*******************************************
				Death zone
Triggers a room restart, and damages the player
upon the player's position entering inside of it.
Trigger's size is a cube with the scale of the 
actor starting from it's position.
********************************************/

// private header
ACTOR_UPDATE(death_zone);
ACTOR_TRANSPARENTDRAWWORLD(death_zone);
static void RestartRoomFromDeath(struct Actor* exit);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(death_zone)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME;
	ACTOR_REGISTER_UPDATE(death_zone);
	ACTOR_REGISTER_TRANSPARENTDRAWWORLD(death_zone);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_UPDATE(death_zone)
{
	struct Actor* fade_actor = FINDACTOR_BYTYPE(act_fadeout);
	if (ACTOR_EXISTS(fade_actor))
	{
		// Wait for fadeout before entering the new scene
		FadeInData* fade_data = fade_actor->data;
		if (fade_data->fadeout >= 255)
		{
			ACTOR_DESTROY(fade_actor);
			RestartRoomFromDeath(actor);
			actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
		}
		return;
	}
	// Wait till we're triggered
	struct Actor* player = FINDACTOR_BYTYPE(act_player);
	if (!ACTOR_EXISTS(player))
		return;
	if (!PointInCube(player->position, actor->position, actor->scale))
		return;

	// Fadeout and reload the current room
	printf("DEATH TRIGGER\n");
	FADEOUT_CREATE(BLACK);
	actor->actor_flags = ACTOR_FLAG_TICKDURING_TRANSITION;
	EXIT_GAMESTATE(GAMESTATE_GAMEPLAY);
	ENTER_GAMESTATE(GAMESTATE_TRANSITION);

	// Lock the camera in place
	struct Actor* camera = FINDACTOR_BYTYPE(act_camera);
	if (ACTOR_EXISTS(camera))
		CameraSetMode(camera, CAMERA_MODE_ONLYWATCH);
}

ACTOR_TRANSPARENTDRAWWORLD(death_zone)
{
	if (!draw_debug_info)
		return;
	DrawCubeWires(Vector3Subtract(actor->position, Vector3Scale(actor->scale, -0.5f)), actor->scale.x, actor->scale.y, actor->scale.z, RED);
}

static void RestartRoomFromDeath(struct Actor* exit)
{
	// Finish transitioning to the newscene
	EXIT_GAMESTATE(GAMESTATE_TRANSITION);
	ENTER_GAMESTATE(GAMESTATE_GAMEPLAY);
	// Triggered death, do damage and restart the current room
	FADEIN_CREATE(BLACK);

	// Kill the player and remake them at a respawn point
	Vector3 respawn_point = Vector3Zero();
	Quaternion respawn_rot = QuaternionIdentity();
	struct Actor* player = FINDACTOR_BYTYPE(act_player);
	if (player)
	{
		respawn_point = player->home_position;
		respawn_rot = player->home_rotation;
	}

	// Reset the scene
	struct Actor* scene = GetCurrentScene();
	ChangeSceneRoom(scene, scene->current_room_index, FALSE, FALSE);
	player = FAST_ACTOR_ROT(act_player, scene, respawn_point, respawn_rot);
	PlayerDamage(QUARTER_HEART, TRUE);

	// Reset the camera
	struct Actor* camera = FINDACTOR_BYTYPE(act_camera);
	if (ACTOR_EXISTS(camera))
	{
		// Snap behind player at entrance
		Vector3 cam_pos = CameraBehindPlayerPosition(player);
		ACTOR_POS_SNAP(camera, cam_pos);
		cam_main.position = cam_pos; // snap this too
		float point_angle = QuaternionToEuler(player->rotation).y + (180.0f * DEG2RAD);
		CameraResetAngleToTarget(camera, point_angle);
		// Restore camera to default state
		CameraSetMode(camera, CAMERA_MODE_FOLLOW);
	}
}
