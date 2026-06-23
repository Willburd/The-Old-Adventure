#include "game_draw.h"
#include "player.h"

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
	// Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_HAS_ANIMATIONS;
	actor->func_preloadassets = actor_player_preload_assets;
	actor->func_update = actor_player_update;
	actor->func_drawworld = actor_player_drawworld;
	actor->func_drawhud = actor_player_drawhud;
	actor->func_destroy = actor_player_destroy;

	// Set data
	MALLOC_ACTOR_DATA(PlayerData, actor->data);
	PlayerData* player_data = (PlayerData*)actor->data;
	player_data->current_state = plysta_grounded;
	PlayerChangeState(actor, plysta_grounded);
	player_data->cutscene_run_goal = Vector3Zero();
	player_data->cutscene_run_factor = 1.0f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The player is a multilayer state machine of polymorphs. It changes behaviors and swaps out functions to suit those states.
// Shared player code is in the Generic playerstate. For things like huds or shared state checks.

static void actor_player_preload_assets(struct Actor* actor)
{

}

static void actor_player_update(struct Actor* actor)
{
	// Update our current state each tick
	PlayerData* player_data = (PlayerData*)actor->data;
	player_data->func_state_update(actor);
}

static void actor_player_drawworld(struct Actor* actor, double tick_percent)
{
	// Draw the player and handle animations
	DRAWCAPSULE(ACTOR_POS_DELTA(actor, tick_percent), 1.0f, 0.5f, GREEN);
	DrawSphere(Vector3Add(ACTOR_POS_DELTA(actor, tick_percent), Vector3Add(Vector3Scale(VEC3UP, 1.9f), Vector3RotateByQuaternion(Vector3Scale(VEC3FORWARD, 0.3f), ACTOR_ROT_DELTA(actor, tick_percent)))), 0.5, BLUE);

	// Additional drawing the state wants
	PlayerData* player_data = (PlayerData*)actor->data;
	player_data->func_state_drawworld(actor, tick_percent);
}

static void actor_player_drawhud(struct Actor* actor, double tick_percent)
{
	// Handle the hud in the state
	PlayerData* player_data = (PlayerData*)actor->data;
	player_data->func_state_drawhud(actor, tick_percent);

	if (!draw_debug_info)
		return;
	DrawText(TextFormat("X:%f\nY:%f\nZ:%f\nA:%f\n", actor->position.x, actor->position.y, actor->position.z, Vector3GetTopDownAngle(Vector3RotateByQuaternion(VEC3FORWARD, actor->rotation)) * RAD2DEG), renderWidth / 2, renderHeight / 2, 4, WHITE);
}

static void actor_player_destroy(struct Actor* actor)
{

}