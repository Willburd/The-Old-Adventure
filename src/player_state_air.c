#include "globals.h"
#include "player.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Airborne player state
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerState_Air_Update(struct Actor* player);
void PlayerState_Air_DrawWorld(struct Actor* player, double tick_percent);
void PlayerState_Air_DrawHud(struct Actor* player, double tick_percent);
void PlayerState_Air_Exit(struct Actor* player);

void PlayerState_Air_Enter(struct Actor* player, PlayerData* player_data, int previous_state)
{
	// Configure state
	player_data->func_state_update = PlayerState_Air_Update;
	player_data->func_state_drawworld = PlayerState_Air_DrawWorld;
	player_data->func_state_drawhud = PlayerState_Air_DrawHud;
	player_data->func_state_exitstate = PlayerState_Air_Exit;
}

void PlayerState_Air_Update(struct Actor* player)
{
	PlayerData* player_data = (PlayerData*)player->data;

	// Pausing
	if (CHECK_INPUTPRESSED(input_pause))
	{
		PlayerStandardPauseActivate(player);
	}

	// slowdown air drift
	ApplyFlatFriction(player, 0.02f);

	// Handle wall collision
	if (!player_data->disable_collision)
	{
		PlayerStandardRadialEjection(player, Vector3Scale(VEC3UP, 0.1f), PLAYER_COLLISION_RADIUS);
		PlayerStandardRadialEjection(player, Vector3Scale(VEC3UP, PLAYER_COLLISION_MID_HEIGHT), PLAYER_COLLISION_RADIUS);
		PlayerStandardRadialEjection(player, Vector3Scale(VEC3UP, PLAYER_COLLISION_TOP_HEIGHT), PLAYER_COLLISION_RADIUS);
	}

	// Handle gravity
	Ray downray = {
		.position = Vector3Add(player->position, Vector3Scale(VEC3UP, PLAYER_COLLISION_MID_HEIGHT)),
		.direction = VEC3DOWN
	};
	RayCollision collision = CollisionGetNearest(downray, PLAYER_COLLISION_MID_HEIGHT * 1.01f, COL_LAYER_WORLD | COL_LAYER_MOVINGPLATFORM);
	if (player_data->disable_collision || !collision.hit)
	{
		// Falling down!
		if (player->velocity.y > PLAYER_TERMINAL_VELOCITY)
			player->velocity.y += GRAVITY;
		return;
	}
	// Snap to floor on landing
	player->position = collision.point;
	PlayerChangeState(player, plysta_grounded);
}

void PlayerState_Air_DrawWorld(struct Actor* player, double tick_percent)
{

}

void PlayerState_Air_DrawHud(struct Actor* player, double tick_percent)
{
	PlayerStandardHudDraw(player, tick_percent);
}

void PlayerState_Air_Exit(struct Actor* player)
{

}