#include <stdio.h>
#include "../tools.h"
#include "../actor_factory.h"
#include "actor_entrance.h"
#include "actor_camera.h"
#include "../collision.h"
#include "../game_draw.h"
#include "../scene_entry.h"
#include "json_properties.h"
#include "player.h"

/*******************************************
				Entrance Node
Entrance spawn point for a player. Each entrance
can be assigned an entrance id. Exits that 
lead to this scene will use that to locate the 
entrance they will use.
See: ENTRANCE_FROM_STRING()

The entrance has an end_pos property, this is
the goal position that player will run to upon
using the entrance. The camera will be placed
directly behind the player in this direction.
********************************************/

// private header
static void actor_entrance_setup(struct Actor* entrance, Vector3 startpos, Vector3 endpos);
ACTOR_JSON_INIT(entrance);
ACTOR_DRAWWORLD(entrance);
static Vector3 actor_entrance_get_start(struct Actor* entrance);
static Vector3 actor_entrance_get_end(struct Actor* entrance);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(entrance)
{
    // Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_TICKDURING_CUTSCENE;
	ACTOR_REGISTER_JSON_INIT(entrance);
	ACTOR_REGISTER_DRAWWORLD(entrance);

	// Set data
	MALLOC_ACTOR_DATA(EntranceData, actor->data);

	// Snap to entrypoint
	ACTOR_POS_SNAP(actor, actor_entrance_get_start(actor));
}

ACTOR_JSON_INIT(entrance)
{
	if (file_data == NULL)
		return;

	Vector3 end_posi;
	JSON_GET_VECTOR3(end_posi, file_data, PROP_ENDPOS, Vector3Zero());

	actor_entrance_setup(actor, actor->position, end_posi);
	EntranceData* entrance_data = actor->data;
	entrance_data->entrance_id = ENTRANCE_FROM_STRING(cJSON_GetObjectItem(file_data, PROP_IS_ENTRANCE)->valuestring);
}

// Perform entrance actions like aligning the camera and making the player run into the scene
void actor_entrance_startentry(struct Actor* entrance)
{
	Vector3 end_pos = actor_entrance_get_end(entrance);
	Vector3 cam_pos = CameraBehindPlayerPosition(entrance);

	struct Actor* player = FINDACTOR_BYTYPE(act_player);
	struct Actor* camera = FINDACTOR_BYTYPE(act_camera);
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
		CameraSetMode(camera, CAMERA_MODE_FOLLOW);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_DRAWWORLD(entrance)
{
	if (!draw_debug_info)
		return;
	DrawCube(actor->position, 0.2f, 0.2f, 0.2f, PURPLE);
	Vector3 end_pos = actor_entrance_get_end(actor);
	DrawLine3D(actor->position, end_pos, PURPLE);
	Vector3 cam_pos = CameraBehindPlayerPosition(actor);
	DrawLine3D(actor->position, cam_pos, GREEN);
}

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
	RayHitData collision = CollisionGetNearest(raycast, 1.0f, COL_LAYER_WORLD);
	if (collision.ray_col.hit)
		return collision.ray_col.point;
	return raycast.position;
}

static Vector3 actor_entrance_get_end(struct Actor* entrance)
{
	Ray raycast = {
		.position = Vector3Add(entrance->position, Vector3RotateByQuaternion(Vector3Scale(VEC3FORWARD, entrance->scale.x), entrance->rotation)),
		.direction = VEC3DOWN
	};
	RayHitData collision = CollisionGetNearest(raycast, 1.0f, COL_LAYER_WORLD);
	if (collision.ray_col.hit)
		return collision.ray_col.point;
	return raycast.position;
}
