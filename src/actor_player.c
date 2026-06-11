#include "tools.h"
#include "assets.h"
#include "actor.h"
#include "input.h"

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
	Vector2 move_dir = input_analog;
	actor->position = Vector3Add(actor->position, Vector3Scale(Vector3RotateByQuaternion((Vector3){ move_dir.x, 0.0f, move_dir.y }, actor->rotation), 0.2f));
}



static void actor_player_drawworld(struct Actor* actor, double tick_percent)
{
	DRAWCAPSULE(ACTOR_POS_DELTA(actor, tick_percent), 1.0f, 0.5f, GREEN);
	DrawSphere(Vector3Add(ACTOR_POS_DELTA(actor, tick_percent), Vector3Add(Vector3Scale(VEC3UP, 1.9f), Vector3RotateByQuaternion(Vector3Scale(VEC3FORWARD, 0.3f), ACTOR_ROT_DELTA(actor, tick_percent)))), 0.5, BLUE);
}

static void actor_player_drawhud(struct Actor* actor, double tick_percent)
{

}

static void actor_player_destroy(struct Actor* actor)
{

}