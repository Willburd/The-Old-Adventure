#include "player.h"
#include "../text_loading.h"
#include "actor_platform.h"
#include "actor_water_volume.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Grounded player state
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PLAYER_GROUND_ACCELERATION 0.04f
#define PLAYER_GROUND_MAXSPEED 0.14f
#define PLAYER_GROUND_FRICTION 0.01f
#define PLAYER_GROUND_STOP_FRICTION 0.06f
#define PLAYER_GROUND_SNAPTURN_FRICTION 0.1f
#define PLAYER_GROUND_TURN_RATE 0.2f

void PlayerState_Death_Update(struct Actor* player);
void PlayerState_Death_DrawWorld(struct Actor* player, double tick_percent);
void PlayerState_Death_DrawHud(struct Actor* player, double tick_percent);
void PlayerState_Death_Exit(struct Actor* player);

void PlayerState_Death_Enter(struct Actor* player, PlayerData* player_data, int previous_state)
{
	// Configure state
	printf("Player death");
	player_data->func_state_update = PlayerState_Death_Update;
	player_data->func_state_drawworld = PlayerState_Death_DrawWorld;
	player_data->func_state_drawhud = PlayerState_Death_DrawHud;
	player_data->func_state_exitstate = PlayerState_Death_Exit;
}

void PlayerState_Death_Update(struct Actor* player)
{
	PlayerData* player_data = (PlayerData*)player->data;
	if (!player_data->disable_collision)
	{
		// Handle gravity
		Ray downray = {
			.position = Vector3Add(player->position, Vector3Scale(VEC3UP, PLAYER_COLLISION_MID_HEIGHT)),
			.direction = VEC3DOWN
		};
		RayHitData collision = CollisionGetNearest(downray, PLAYER_COLLISION_MID_HEIGHT + PLAYER_COLLISION_FLOOR_SENSOR_LENGTH, COL_LAYER_WORLD | COL_LAYER_MOVINGPLATFORM);
		if (collision.ray_col.hit) 
		{
			// Snap to floors
			player->position = collision.ray_col.point;
			if (collision.hit_colider->flags & COL_LAYER_MOVINGPLATFORM) // Moving platforms make us move too
				ApplyPlatformRotation(player, collision.hit_colider->owner, TRUE);
		}
		else
		{
			// We must fall...
			PlayerApplyGravity(player);
		}
	}
}

void PlayerState_Death_DrawWorld(struct Actor* player, double tick_percent)
{

}

void PlayerState_Death_DrawHud(struct Actor* player, double tick_percent)
{

}

void PlayerState_Death_Exit(struct Actor* player)
{

}