#include "tools.h"
#include "assets.h"
#include "actor.h"

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

}

static void actor_player_drawworld(struct Actor* actor, double tick_percent)
{
	DrawCube(actor->position, 0.2f, 0.2f, 0.2f, GREEN);
}

static void actor_player_drawhud(struct Actor* actor, double tick_percent)
{

}

static void actor_player_destroy(struct Actor* actor)
{

}