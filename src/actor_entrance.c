#include <stdio.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_entrance.h"
#include "camera.h"

// private header
void actor_entrance_drawworld(struct Actor* entrance, double delta_time);
Vector3 actor_entrance_get_start(struct Actor* entrance);
Vector3 actor_entrance_get_end(struct Actor* entrance);
Vector3 actor_entrance_get_camerastart(struct Actor* entrance);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void actor_entrance_init(struct Actor* actor)
{
    // Configure actor
#ifdef _DEBUG
	actor->func_drawworld = actor_entrance_drawworld;
#endif

	// Set data
	MALLOC_ACTOR_DATA(EntranceData, actor->data);
}

// Perform entrance actions like aligning the camera and making the player run into the scene
void actor_entrance_startentry(struct Actor* entrance)
{
	Vector3 spawn_pos = actor_entrance_get_start(entrance);
	Vector3 end_pos = actor_entrance_get_end(entrance);
	Vector3 cam_pos = actor_entrance_get_camerastart(entrance);

	struct Actor* player = FINDACTORTYPE(act_player);
	struct Actor* camera = FINDACTORTYPE(act_camera);
	if (ACTOR_EXISTS(camera))
	{
		ACTOR_POS_SNAP(camera, cam_pos);
	}
	if (ACTOR_EXISTS(player))
	{
		ACTOR_POS_SNAP(player, spawn_pos);
		ACTOR_ROT_SNAP(player, QuaternionFromAxisAngle(VEC3UP, Vector3Angle(spawn_pos, end_pos)));
		if (ACTOR_EXISTS(camera)) // Focus on player from camera pos
			cam_main.target = Vector3Add(spawn_pos, VEC3UP);
	}
}

void actor_entrance_setup(struct Actor* entrance, Vector3 startpos, Vector3 endpos)
{
	ACTOR_POS_SNAP(entrance, startpos);
	// Stay on same plane for rotations
	Vector3 dir_vec = (Vector3){ endpos.x, startpos.y, endpos.z }; 
	entrance->scale.x = Vector3Distance(startpos, dir_vec);
	ACTOR_ROT_SNAP(entrance, QuaternionNormalize(QuaternionFromVector3ToVector3(startpos, dir_vec)));
}

struct Actor* entrance_create(int entrance_id, struct Actor* scene, Vector3 s_pos, Vector3 e_pos)
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
Vector3 actor_entrance_get_start(struct Actor* entrance)
{
	Vector3 ray_check_pos = entrance->position;
	// TODO - Raycast to the ground
	return ray_check_pos;
}

Vector3 actor_entrance_get_end(struct Actor* entrance)
{
	Vector3 offset = Vector3Scale(VEC3FORWARD, entrance->scale.x);
	Vector3 ray_check_pos = Vector3Add(entrance->position, Vector3RotateByQuaternion(offset, entrance->rotation));
	// TODO - Raycast to the ground
	return ray_check_pos;
}

Vector3 actor_entrance_get_camerastart(struct Actor* entrance)
{
	Vector3 offset = Vector3Scale(VEC3BACKWARD, 2.0f);
	offset = Vector3Add(offset, Vector3Scale(VEC3UP, 2.0f));
	return Vector3Add(entrance->position, Vector3RotateByQuaternion(offset, entrance->rotation));
}

void actor_entrance_drawworld(struct Actor* entrance, double delta_time)
{
	DrawCube(entrance->position, 0.2f, 0.2f, 0.2f, PURPLE);
	Vector3 end_pos = actor_entrance_get_end(entrance);
	DrawLine3D(entrance->position, end_pos, PURPLE);
	Vector3 cam_pos = actor_entrance_get_camerastart(entrance);
	DrawLine3D(entrance->position, cam_pos, GREEN);
}