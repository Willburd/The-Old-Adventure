#include "player.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Grounded player state
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PLAYER_GROUND_ACCELERATION 0.03f
#define PLAYER_GROUND_MAXSPEED 0.1f
#define PLAYER_GROUND_STOP_FRICTION 0.2f
#define PLAYER_GROUND_SNAPTURN_FRICTION 0.7f

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
		Vector3 move_velocity = Vector3Scale(Vector3RotateByQuaternion((Vector3) { move_dir.x, 0.0f, move_dir.y }, player->rotation), PLAYER_GROUND_ACCELERATION);

		if (Vector3Length(move_velocity) > 0.01f)
		{
			float direction_moving_dot = 0.0f;
			if (Vector2Length((Vector2) { player->velocity.x, player->velocity.z }) >= PLAYER_GROUND_MAXSPEED)
			{
				direction_moving_dot = Vector2DotProduct((Vector2) { move_velocity.x, move_velocity.z }, (Vector2) { player->velocity.x, player->velocity.z });
				if (direction_moving_dot < 0) // Hard stop
				{
					ApplyFriction(player, PLAYER_GROUND_SNAPTURN_FRICTION);
				}
			}
			// Apply velocity, if we are at maximum speed, only apply a fraction of it based on if we are moving toward the velocity already
			direction_moving_dot = Clamp(direction_moving_dot, 0.0f, 1.0f); // Only care about if we are moving in the same direction
			player->velocity = Vector3Add(player->velocity, Vector3Scale(move_velocity, 1.0f - direction_moving_dot));
		}
		else
		{
			// Slowdown pver time if not moving
			ApplyFriction(player, PLAYER_GROUND_STOP_FRICTION);
		}
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