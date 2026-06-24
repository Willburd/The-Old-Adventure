#include <stdio.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_entrance.h"
#include "camera.h"
#include "collision.h"
#include "game_draw.h"

// private header
static void actor_entrance_setup(struct Actor* entrance, Vector3 startpos, Vector3 endpos);
ACTOR_DRAWWORLD(entrance);
static Vector3 actor_entrance_get_start(struct Actor* entrance);
static Vector3 actor_entrance_get_end(struct Actor* entrance);
static Vector3 actor_entrance_get_camerastart(struct Actor* entrance);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(entrance)
{
    // Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_TICKDURING_CUTSCENE;
	ACTOR_REGISTER_DRAWWORLD(entrance);

	// Set data
	MALLOC_ACTOR_DATA(EntranceData, actor->data);

	// Snap to entrypoint
	ACTOR_POS_SNAP(actor, actor_entrance_get_start(actor));
}

// Perform entrance actions like aligning the camera and making the player run into the scene
void actor_entrance_startentry(struct Actor* entrance)
{
	Vector3 end_pos = actor_entrance_get_end(entrance);
	Vector3 cam_pos = actor_entrance_get_camerastart(entrance);

	struct Actor* player = FINDACTORTYPE(act_player);
	struct Actor* camera = FINDACTORTYPE(act_camera);
	if (ACTOR_EXISTS(player))
	{
		ACTOR_POS_SNAP(player, entrance->position);
		ACTOR_ROT_SNAP(player, entrance->rotation);
	}
	if (ACTOR_EXISTS(camera))
	{
		// Snap behind player at entrance
		ACTOR_POS_SNAP(camera, cam_pos);
		cam_main.position = cam_pos; // snap this too
		float point_angle =	QuaternionToEuler(entrance->rotation).y + (180.0f * DEG2RAD);
		CameraResetAngleToTarget(camera, point_angle);
		// Restore camera to default state
		CameraData* cam_data = (CameraData*)camera->data;
		cam_data->camera_mode = CAMERA_MODE_FOLLOW;
	}
}

struct Actor* ENTRANCE_CREATE(int entrance_id, struct Actor* scene, Vector3 s_pos, Vector3 e_pos)
{
	struct Actor* entrance = ACTOR_FACTORY(act_entrance, scene, s_pos, QuaternionIdentity(), Vector3One(), Vector3Zero());
	actor_entrance_setup(entrance, s_pos, e_pos);
	EntranceData* entrance_data = entrance->data;
	entrance_data->entrance_id = entrance_id;
	return entrance;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void actor_entrance_setup(struct Actor* entrance, Vector3 startpos, Vector3 endpos)
{
	// Stay on same plane for rotations
	entrance->scale.x = Vector3Distance(startpos, endpos);
	ACTOR_ROT_SNAP(entrance, QuaternionFromAxisAngle(VEC3UP, Vector3GetTopDownAngle(VEC3DIRECTION(startpos, endpos))));
}

static Vector3 actor_entrance_get_start(struct Actor* entrance)
{
	Ray raycast = {
		.position = entrance->position,
		.direction = VEC3DOWN
	};
	RayCollision collision = CollisionGetNearest(raycast, 1.0f, COL_LAYER_WORLD);
	if (collision.hit)
		return collision.point;
	return raycast.position;
}

static Vector3 actor_entrance_get_end(struct Actor* entrance)
{
	Ray raycast = {
		.position = Vector3Add(entrance->position, Vector3RotateByQuaternion(Vector3Scale(VEC3FORWARD, entrance->scale.x), entrance->rotation)),
		.direction = VEC3DOWN
	};
	RayCollision collision = CollisionGetNearest(raycast, 1.0f, COL_LAYER_WORLD);
	if (collision.hit)
		return collision.point;
	return raycast.position;
}

static Vector3 actor_entrance_get_camerastart(struct Actor* entrance)
{
	Vector3 offset = Vector3Scale(VEC3BACKWARD, 5.0f);
	offset = Vector3Add(offset, Vector3Scale(VEC3UP, 3.0f));
	return Vector3Add(entrance->position, Vector3RotateByQuaternion(offset, entrance->rotation));
}

ACTOR_DRAWWORLD(entrance)
{
	if (!draw_debug_info)
		return;
	DrawCube(actor->position, 0.2f, 0.2f, 0.2f, PURPLE);
	Vector3 end_pos = actor_entrance_get_end(actor);
	DrawLine3D(actor->position, end_pos, PURPLE);
	Vector3 cam_pos = actor_entrance_get_camerastart(actor);
	DrawLine3D(actor->position, cam_pos, GREEN);
}