#include "player.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Player state control
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CREATE_STATE(state) \
void PlayerState_##state##_Enter(struct Actor* player, PlayerData* player_data, int previous_state); \
void PlayerState_##state##_Update(struct Actor* player); \
void PlayerState_##state##_DrawWorld(struct Actor* player, double tick_percent); \
void PlayerState_##state##_DrawHud(struct Actor* player, double tick_percent); \
void PlayerState_##state##_Exit(struct Actor* player)

CREATE_STATE(Generic); // Misc state just for helpers
CREATE_STATE(Grounded); // When on the ground and moving
CREATE_STATE(Air); // Falling through the air

#define STATE_ENTER(id, state) \
case id: \
	PlayerState_##state##_Enter(player, player_data, old_state); \
	return

// State entry and tick functions
void PlayerChangeState(struct Actor* player, int new_state)
{
	// Eject the previous state if we are changing to a new one
	PlayerData* player_data = (PlayerData*)player->data;
	int old_state = player_data->current_state;
	if (player_data->current_state != new_state)
		player_data->func_state_exitstate(player);

	// Change to a new state
	switch (new_state)
	{
		STATE_ENTER(plysta_grounded, Grounded);
		STATE_ENTER(plysta_air, Air);
	}
}

#undef CREATE_STATE
#undef STATE_ENTER

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generic player state
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerState_Generic_Update(struct Actor* player);
void PlayerState_Generic_DrawWorld(struct Actor* player, double tick_percent);
void PlayerState_Generic_DrawHud(struct Actor* player, double tick_percent);
void PlayerState_Generic_Exit(struct Actor* player);

void PlayerState_Generic_Enter(struct Actor* player, PlayerData* player_data, int previous_state)
{

}

void PlayerState_Generic_Update(struct Actor* player)
{

}

void PlayerState_Generic_DrawWorld(struct Actor* player, double tick_percent)
{

}

void PlayerState_Generic_DrawHud(struct Actor* player, double tick_percent)
{

}

void PlayerState_Generic_Exit(struct Actor* player)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Player utility functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int PlayerCanAcceptInput(struct Actor* player)
{
	struct Actor* camera = FINDACTORTYPE(act_camera);
	CameraData* cam_data = (CameraData*)camera->data;

	int can_accept_player_input = TRUE;
	if (cam_data->camera_mode == CAMERA_MODE_FREEMOVE)
		can_accept_player_input = FALSE;
	if (!(gameplay_state & GAMESTATE_GAMEPLAY))
		can_accept_player_input = FALSE;

	return can_accept_player_input;
}