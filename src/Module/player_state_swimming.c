#include "../globals.h"
#include "player.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Swimming player state
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PLAYER_SWIM_ACCELERATION 0.04f
#define PLAYER_SWIM_MAXSPEED 0.1f
#define PLAYER_SWIM_FRICTION 0.001f
#define PLAYER_SWIM_STOP_FRICTION 0.1f
#define PLAYER_SWIM_TURN_RATE 0.08f

void PlayerState_Swimming_Update(struct Actor* player);
void PlayerState_Swimming_DrawWorld(struct Actor* player, double tick_percent);
void PlayerState_Swimming_DrawHud(struct Actor* player, double tick_percent);
void PlayerState_Swimming_Exit(struct Actor* player);

void PlayerState_Swimming_Enter(struct Actor* player, PlayerData* player_data, int previous_state)
{
	// Configure state
	player_data->func_state_update = PlayerState_Swimming_Update;
	player_data->func_state_drawworld = PlayerState_Swimming_DrawWorld;
	player_data->func_state_drawhud = PlayerState_Swimming_DrawHud;
	player_data->func_state_exitstate = PlayerState_Swimming_Exit;

	// Sudden slowdown of velocity when hitting water, sploosh!
	player->velocity = Vector3Scale(player->velocity, 0.1f);
}

void PlayerState_Swimming_Update(struct Actor* player)
{
	int can_accept_input = PlayerCanAcceptInput(player);
	PlayerStandardBehavior(player, can_accept_input,
		PLAYER_SWIM_MAXSPEED,
		PLAYER_SWIM_ACCELERATION,
		PLAYER_SWIM_FRICTION,
		PLAYER_SWIM_STOP_FRICTION,
		PLAYER_SWIM_STOP_FRICTION,
		PLAYER_SWIM_TURN_RATE);

	// Handle wall collision
	PlayerData* player_data = (PlayerData*)player->data;
	if (!player_data->disable_collision)
	{
		PlayerStandardRadialEjection(player, Vector3Scale(VEC3UP, PLAYER_COLLISION_STEP_HEIGHT), PLAYER_COLLISION_RADIUS);
		PlayerStandardRadialEjection(player, Vector3Scale(VEC3UP, PLAYER_COLLISION_MID_HEIGHT), PLAYER_COLLISION_RADIUS);
		PlayerStandardRadialEjection(player, Vector3Scale(VEC3UP, PLAYER_COLLISION_TOP_HEIGHT), PLAYER_COLLISION_RADIUS);
	}

	// Handle floor detection, if we're on a floor while swimming, we're probably no longer swimming
	Ray downray = {
		.position = Vector3Add(player->position, Vector3Scale(VEC3UP, PLAYER_COLLISION_MID_HEIGHT)),
		.direction = VEC3DOWN
	};
	RayHitData collision = CollisionGetNearest(downray, PLAYER_COLLISION_MID_HEIGHT, COL_LAYER_WORLD | COL_LAYER_MOVINGPLATFORM);
	if (!player_data->disable_collision && collision.ray_col.hit)
	{
		// Snap to floors if we were going downward
		player->position = collision.ray_col.point;

		// We're touching the ground, check if we're also not submerged up to our midpoint...
		if (!PointInWaterVolume(Vector3Add(player->position, Vector3Scale(VEC3UP, PLAYER_SWIM_HEIGHT))))
		{
			PlayerChangeState(player, plysta_grounded);
			return;
		}
	}
	else
	{
		// If this becomes unsubmerged then we are out of water for sure, but in the air because of no collision beneath us...
		if (!PointInWaterVolume(Vector3Add(player->position, Vector3Scale(VEC3UP, PLAYER_COLLISION_STEP_HEIGHT))))
		{
			PlayerChangeState(player, plysta_air);
			return;
		}
	}

	// If our water points is not submerged we need to move downward, if it is submerged move upward!
	static float boyant_speed = 0.001;
	if (PointInWaterVolume(Vector3Add(player->position, Vector3Scale(VEC3UP, PLAYER_SWIM_HEIGHT))))
		player->velocity.y += boyant_speed; // Submerged, go up
	else
		player->velocity.y -= boyant_speed; // airborne, go down
	// But stay under a speed cap
	static float bouyant_cap = 0.2f;
	float bynt_velocity = abs(player->velocity.y);
	if (bynt_velocity > bouyant_cap)
	{
		if (bynt_velocity > 0)
			player->velocity.y = bouyant_cap;
		else
			player->velocity.y = -bouyant_cap;
	}

	// Handle interactions
	PlayerStandardInteraction(player, can_accept_input);
}

void PlayerState_Swimming_DrawWorld(struct Actor* player, double tick_percent)
{

}

void PlayerState_Swimming_DrawHud(struct Actor* player, double tick_percent)
{
	PlayerStandardHudDraw(player, tick_percent);
}

void PlayerState_Swimming_Exit(struct Actor* player)
{

}