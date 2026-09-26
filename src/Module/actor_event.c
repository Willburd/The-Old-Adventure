#include "../actor_factory.h"
#include "../tools.h"
#include "json_properties.h"
#include "../camera.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set flag event
// Set flags for the current scene, uses this actor's flag_group_selector and triggers_flags.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_REMOTE_INTERACT(event_setflag);
ACTOR_INIT(event_setflag) {
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK | ACTOR_FLAG_IS_INVISIBLE;
	ACTOR_REGISTER_REMOTE_INTERACT(event_setflag);
}
ACTOR_REMOTE_INTERACT(event_setflag)
{
	SceneFlagTrigger(actor);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Clear flag event
// Clears flags for the current scene, uses this actor's flag_group_selector and triggers_flags.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_REMOTE_INTERACT(event_clearflag);
ACTOR_INIT(event_clearflag) {
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK | ACTOR_FLAG_IS_INVISIBLE;
	ACTOR_REGISTER_REMOTE_INTERACT(event_clearflag);
}
ACTOR_REMOTE_INTERACT(event_clearflag)
{
	SceneFlagClear(actor);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Toggle flag event
// Toggles flags for the current scene, uses this actor's flag_group_selector and triggers_flags.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_REMOTE_INTERACT(event_toggleflag);
ACTOR_INIT(event_toggleflag) {
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK | ACTOR_FLAG_IS_INVISIBLE;
	ACTOR_REGISTER_REMOTE_INTERACT(event_toggleflag);
}
ACTOR_REMOTE_INTERACT(event_toggleflag)
{
	SceneFlagToggle(actor);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// X event
// Description
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
	float percent;
	float speed;
	Vector3 start_pos;
	Quaternion start_rot;
} CameraMoveData;

ACTOR_REMOTE_INTERACT(event_movecamera);
ACTOR_JSON_INIT(event_movecamera);
ACTOR_UPDATE(event_movecamera);
ACTOR_INIT(event_movecamera) {
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_TEXTBOX | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_IS_INVISIBLE;
	ACTOR_REGISTER_JSON_INIT(event_movecamera);
	ACTOR_REGISTER_REMOTE_INTERACT(event_movecamera);
	ACTOR_REGISTER_UPDATE(event_movecamera);

	MALLOC_ACTOR_DATA(CameraMoveData, actor->data);
	CameraMoveData* camera_data = (CameraMoveData*)actor->data;
	camera_data->percent = 0.0f;
	camera_data->speed = 1.0f;
	camera_data->start_pos = Vector3Zero();
	camera_data->start_rot = QuaternionIdentity();
}
ACTOR_JSON_INIT(event_movecamera)
{
	if (file_data == NULL) 
		return;
	CameraMoveData* camera_data = (CameraMoveData*)actor->data;
	JSON_GET_FLOAT(camera_data->speed, file_data, PROP_PLATFORM_CAMERASPEED, 1.0f);
}
ACTOR_REMOTE_INTERACT(event_movecamera)
{
	CameraMoveData* camera_data = (CameraMoveData*)actor->data;
	if (camera_data->percent > 0)
		return;
	struct Actor* camera = FINDACTOR_BYTYPE(act_camera);
	if (!camera)
		return;
	camera_data->percent = 0.001f;
	camera_data->start_pos = camera->position;
	camera_data->start_rot = camera->rotation;
}
ACTOR_UPDATE(event_movecamera)
{
	CameraMoveData* camera_data = (CameraMoveData*)actor->data;
	if (camera_data->percent <= 0)
		return;
	struct Actor* camera = FINDACTOR_BYTYPE(act_camera);
	if (!camera)
		return;

	// Move the cameras as needed
	if (camera_data->speed < 1000.0f)
	{
		float speed_percent = camera_data->speed / Vector3Distance(camera_data->start_pos, actor->position);
		camera_data->percent = Clamp(camera_data->percent + speed_percent, 0.0f, 1.0f);
		camera->position = Vector3Lerp(camera_data->start_pos, actor->position, camera_data->percent);
		camera->rotation = QuaternionSlerp(camera_data->start_rot, actor->rotation, camera_data->percent);
	}
	else
	{ 
		camera_data->percent = 1.0f;
		ACTOR_POS_SNAP(camera, actor->position);
		ACTOR_ROT_SNAP(camera, actor->rotation);
	}

	// End camera control
	if (camera_data->percent < 1.0f)
		return;
	camera_data->percent = 0.0f;
}

