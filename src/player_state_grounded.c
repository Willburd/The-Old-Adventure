#include "player.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Grounded player state
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerState_Grounded_Update(struct Actor* player);
void PlayerState_Grounded_DrawWorld(struct Actor* player, double tick_percent);
void PlayerState_Grounded_DrawHud(struct Actor* player, double tick_percent);
void PlayerState_Grounded_Exit(struct Actor* player);

void PlayerState_Grounded_Enter(struct Actor* player, PlayerData* player_data, int previous_state)
{
	// Configure state
	player_data->func_state_update = PlayerState_Grounded_Update;
	player_data->func_state_drawworld = PlayerState_Grounded_DrawWorld;
	player_data->func_state_drawhud = PlayerState_Grounded_DrawHud;
	player_data->func_state_exitstate = PlayerState_Grounded_Exit;
}

void PlayerState_Grounded_Update(struct Actor* player)
{
	// Handle player inputs
	if (PlayerCanAcceptInput(player))
	{
		Vector2 move_dir = input_analog;
		float test_angle = Vector3GetTopDownAngle(VEC3DIRECTION(cam_main.position, player->position));
		Vector3 move_velocity = Vector3Scale(Vector3RotateByQuaternion((Vector3) { move_dir.x, 0.0f, move_dir.y }, player->rotation), 0.2f);

	}
}

void PlayerState_Grounded_DrawWorld(struct Actor* player, double tick_percent)
{

}

void PlayerState_Grounded_DrawHud(struct Actor* player, double tick_percent)
{

}

void PlayerState_Grounded_Exit(struct Actor* player)
{

}