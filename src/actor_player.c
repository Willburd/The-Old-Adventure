#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "input.h"
#include "camera.h"

// private header
static void actor_player_preload_assets(struct Actor* actor);
static void actor_player_update(struct Actor* actor);
static void actor_player_drawworld(struct Actor* actor, double delta_time);
static void actor_player_drawhud(struct Actor* actor, double delta_time);
static void actor_player_destroy(struct Actor* actor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void actor_player_init(struct Actor* actor)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_HAS_ANIMATIONS;
	actor->func_preloadassets = actor_player_preload_assets;
	actor->func_update = actor_player_update;
	actor->func_drawworld = actor_player_drawworld;
	actor->func_drawhud = actor_player_drawhud;
	actor->func_destroy = actor_player_destroy;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void actor_player_preload_assets(struct Actor* actor)
{

}

static void actor_player_update(struct Actor* actor)
{
	struct Actor* camera = FINDACTORTYPE(act_camera);
	CameraData* cam_data = (CameraData*)camera->data;

	if (cam_data->camera_mode == CAMERA_MODE_FREEMOVE)
		return;

	Vector2 move_dir = input_analog;
	float test_angle = Vector3GetTopDownAngle(VEC3DIRECTION(cam_main.position, actor->position));
	actor->position = Vector3Add(actor->position, Vector3Scale(Vector3RotateByQuaternion((Vector3){ move_dir.x, 0.0f, move_dir.y }, actor->rotation), 0.2f));
}



static void actor_player_drawworld(struct Actor* actor, double tick_percent)
{
	DRAWCAPSULE(ACTOR_POS_DELTA(actor, tick_percent), 1.0f, 0.5f, GREEN);
	DrawSphere(Vector3Add(ACTOR_POS_DELTA(actor, tick_percent), Vector3Add(Vector3Scale(VEC3UP, 1.9f), Vector3RotateByQuaternion(Vector3Scale(VEC3FORWARD, 0.3f), ACTOR_ROT_DELTA(actor, tick_percent)))), 0.5, BLUE);

	DrawLine3D(actor->position, Vector3Add(actor->position, VEC3FORWARD), RED);
}

static void actor_player_drawhud(struct Actor* actor, double tick_percent)
{

}

static void actor_player_destroy(struct Actor* actor)
{

}