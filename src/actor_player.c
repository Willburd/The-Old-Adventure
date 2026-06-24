#include "game_draw.h"
#include "player.h"

// private header
ACTOR_PRELOADASSETS(player);
ACTOR_UPDATE(player);
ACTOR_DRAWWORLD(player);
ACTOR_DRAWHUD(player);
ACTOR_CLEANUP(player);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
ACTOR_INIT(player)
{
	// Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_HAS_ANIMATIONS;
	ACTOR_REGISTER_PRELOADASSETS(player);
	ACTOR_REGISTER_UPDATE(player);
	ACTOR_REGISTER_DRAWWORLD(player);
	ACTOR_REGISTER_DRAWHUD(player);
	ACTOR_REGISTER_CLEANUP(player);

	// Set data
	MALLOC_ACTOR_DATA(PlayerData, actor->data);
	PlayerData* player_data = (PlayerData*)actor->data;
	player_data->current_state = plysta_grounded;
	PlayerChangeState(actor, plysta_grounded);
	player_data->cutscene_run_goal = Vector3Zero();
	player_data->cutscene_run_factor = 1.0f;
	player_data->current_action_button_text = "";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The player is a multilayer state machine of polymorphs. It changes behaviors and swaps out functions to suit those states.
// Shared player code is in the Generic playerstate. For things like huds or shared state checks.

ACTOR_PRELOADASSETS(player)
{

}

ACTOR_UPDATE(player)
{
	// Update our current state each tick
	PlayerData* player_data = (PlayerData*)actor->data;
	player_data->func_state_update(actor);
}

ACTOR_DRAWWORLD(player)
{
	// Draw the player and handle animations
	DRAWCAPSULE(ACTOR_POS_DELTA(actor, tick_percent), 1.0f, 0.5f, GREEN);
	DrawSphere(Vector3Add(ACTOR_POS_DELTA(actor, tick_percent), Vector3Add(Vector3Scale(VEC3UP, 1.9f), Vector3RotateByQuaternion(Vector3Scale(VEC3FORWARD, 0.3f), ACTOR_ROT_DELTA(actor, tick_percent)))), 0.5, BLUE);

	// Additional drawing the state wants
	PlayerData* player_data = (PlayerData*)actor->data;
	player_data->func_state_drawworld(actor, tick_percent);
}

ACTOR_DRAWHUD(player)
{
	// Handle the hud in the state
	PlayerData* player_data = (PlayerData*)actor->data;
	player_data->func_state_drawhud(actor, tick_percent);

	if (!draw_debug_info)
		return;
	DrawText(TextFormat("X:%f\nY:%f\nZ:%f\nA:%f\n", actor->position.x, actor->position.y, actor->position.z, Vector3GetTopDownAngle(Vector3RotateByQuaternion(VEC3FORWARD, actor->rotation)) * RAD2DEG), renderWidth / 2, renderHeight / 2, 4, WHITE);
}

ACTOR_CLEANUP(player)
{

}