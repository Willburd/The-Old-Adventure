#include "player.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Grounded player state
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PLAYER_GROUND_ACCELERATION 0.03f
#define PLAYER_GROUND_MAXSPEED 0.20f
#define PLAYER_GROUND_STOP_FRICTION 0.2f
#define PLAYER_GROUND_SNAPTURN_FRICTION 0.7f
#define PLAYER_GROUND_TURN_RATE 0.3f

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
	Vector3 move_velocity = { 0 };
	if (PlayerCanAcceptInput(player))
	{
		Quaternion input_rotator = QuaternionFromAxisAngle(VEC3UP, -Vector3GetTopDownAngle(VEC3DIRECTION(cam_main.position, player->position)));
		move_velocity = Vector3Scale(Vector3RotateByQuaternion((Vector3) { input_analog.x, 0.0f, input_analog.y }, input_rotator), PLAYER_GROUND_ACCELERATION);
	}

	// Slowdown over time if not moving.
	if (Vector3Length(move_velocity) < 0.01f)
	{
		ApplyFriction(player, PLAYER_GROUND_STOP_FRICTION);
		return;
	}

	// Move as directed
	float direction_moving_dot = Vector2DotProduct((Vector2) { move_velocity.x, move_velocity.z }, (Vector2) { player->velocity.x, player->velocity.z });
	if (direction_moving_dot < -0.25) // Hard stop, changing direction.
		ApplyFriction(player, PLAYER_GROUND_SNAPTURN_FRICTION);
	else
		ApplyFriction(player, 0.02); // Always apply some slowing.

	// Accelerate up to full!
	player->velocity = Vector3Add(player->velocity, move_velocity);

	// Slow the player back down if they go over the cap speed.
	Vector2 flat_velocity = (Vector2){ player->velocity.x, player->velocity.z };
	if (Vector2Length(flat_velocity) > PLAYER_GROUND_MAXSPEED)
	{
		Vector2 dirvec = Vector2Scale(Vector2Normalize(flat_velocity), PLAYER_GROUND_MAXSPEED);
		player->velocity.x = dirvec.x;
		player->velocity.z = dirvec.y;
	}

	// Rotate the player toward the direction being moved
	if (Vector2Length(flat_velocity) > 0.0f)
	{
		Vector2 dirvec = Vector2Normalize(flat_velocity);

		Vector3 facing_dir = Vector3RotateByQuaternion(VEC3FORWARD, player->rotation);
		Vector2 flat_facing = Vector2Normalize((Vector2){ facing_dir.x, facing_dir.z });

		float turn_modifier = 1.0f;
		float angle_modifier = Vector2Angle(dirvec, flat_facing);
		if (abs(angle_modifier * RAD2DEG) <= 10.0f) // Slower rotation when almost facing the angle
			turn_modifier *= 0.5f;
		if (abs(angle_modifier * RAD2DEG) <= 5.0f) // Even slower
			turn_modifier *= 0.5f;
		if (abs(angle_modifier * RAD2DEG) > 1.0f) // We're on target
			player->rotation = QuaternionMultiply(player->rotation, QuaternionFromAxisAngle(VEC3UP, SIGN(angle_modifier) * turn_modifier * PLAYER_GROUND_TURN_RATE));
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