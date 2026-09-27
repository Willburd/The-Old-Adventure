#include "player.h"
#include "../text_loading.h"
#include "actor_platform.h"
#include "actor_water_volume.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Grounded player state
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PLAYER_GROUND_ACCELERATION 0.08f
#define PLAYER_GROUND_MAXSPEED 0.21f
#define PLAYER_GROUND_FRICTION 0.05f
#define PLAYER_GROUND_STOP_FRICTION 0.2f
#define PLAYER_GROUND_SNAPTURN_FRICTION 0.7f
#define PLAYER_GROUND_TURN_RATE 0.2f

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

	// Stop falling
	player->velocity.y = 0.0f;
}

void PlayerState_Grounded_Update(struct Actor* player)
{
	int can_accept_input = PlayerCanAcceptInput(player);
	PlayerStandardBehavior(player, can_accept_input,
							PLAYER_GROUND_MAXSPEED, 
							PLAYER_GROUND_ACCELERATION, 
							PLAYER_GROUND_FRICTION, 
							PLAYER_GROUND_STOP_FRICTION, 
							PLAYER_GROUND_SNAPTURN_FRICTION, 
							PLAYER_GROUND_TURN_RATE);

	// Handle wall collision
	PlayerData* player_data = (PlayerData*)player->data;
	if (!player_data->disable_collision)
	{
		PlayerStandardRadialEjection(player, Vector3Scale(VEC3UP, PLAYER_COLLISION_STEP_HEIGHT), PLAYER_COLLISION_RADIUS);
		PlayerStandardRadialEjection(player, Vector3Scale(VEC3UP, PLAYER_COLLISION_MID_HEIGHT), PLAYER_COLLISION_RADIUS);
		PlayerStandardRadialEjection(player, Vector3Scale(VEC3UP, PLAYER_COLLISION_TOP_HEIGHT), PLAYER_COLLISION_RADIUS);
	}

	// Check if we should enter the water
	if (PointInWaterVolume(Vector3Add(player->position, Vector3Scale(VEC3UP, PLAYER_SWIM_HEIGHT))))
	{
		// We must fall...
		PlayerChangeState(player, plysta_swimming);
		return;
	}

	// Handle gravity
	Ray downray = {
		.position = Vector3Add(player->position, Vector3Scale(VEC3UP, PLAYER_COLLISION_MID_HEIGHT)),
		.direction = VEC3DOWN
	};
	RayHitData collision = CollisionGetNearest(downray, PLAYER_COLLISION_MID_HEIGHT + PLAYER_COLLISION_FLOOR_SENSOR_LENGTH, COL_LAYER_WORLD | COL_LAYER_MOVINGPLATFORM);
	if (player_data->disable_collision || !collision.ray_col.hit)
	{
		// We must fall...
		PlayerChangeState(player, plysta_air);
		return;
	}

	// Snap to floors and go up steps
	player->position = collision.ray_col.point;
	if (collision.hit_colider->flags & COL_LAYER_MOVINGPLATFORM) // Moving platforms make us move too
		ApplyPlatformRotation(player, collision.hit_colider->owner, TRUE);

	// Handle interactions
	PlayerStandardInteraction(player, can_accept_input);
}

void PlayerState_Grounded_DrawWorld(struct Actor* player, double tick_percent)
{

}

void PlayerState_Grounded_DrawHud(struct Actor* player, double tick_percent)
{
	PlayerStandardHudDraw(player, tick_percent);
}

void PlayerState_Grounded_Exit(struct Actor* player)
{

}