#include "player.h"
#include "../text_loading.h"
#include "actor_platform.h"

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
	PlayerData* player_data = (PlayerData*)player->data;

	Vector3 move_velocity = { 0 };
	int can_accept_input = PlayerCanAcceptInput(player);
	if (can_accept_input)
	{
		// Pausing
		if (CHECK_INPUTPRESSED(input_pause))
		{
			PlayerStandardPauseActivate(player);
		}

		// Handle player inputs
		Quaternion input_rotator = QuaternionFromAxisAngle(VEC3UP, -Vector3GetTopDownAngle(VEC3DIRECTION(cam_main.position, player->position)));
		move_velocity = Vector3Scale(Vector3RotateByQuaternion((Vector3) { input_analog.x, 0.0f, input_analog.y }, input_rotator), PLAYER_GROUND_ACCELERATION);
	}
	else if (CHECK_GAMESTATE(GAMESTATE_TRANSITION | GAMESTATE_CUTSCENE))
	{
		// Cutscene movement, use the rungoal vector
		if (player_data->cutscene_run_goal.x != 0 || player_data->cutscene_run_goal.z != 0)
			move_velocity = Vector3Scale(Vector3FlatDirection(player->position, player_data->cutscene_run_goal), PLAYER_GROUND_ACCELERATION * player_data->cutscene_run_factor);
	}

	// Slowdown over time if not moving.
	if (Vector3Length(move_velocity) < 0.01f)
		ApplyFriction(player, PLAYER_GROUND_STOP_FRICTION);

	// Move as directed
	int snap_turn = FALSE;
	float direction_moving_dot = Vector2DotProduct((Vector2) { move_velocity.x, move_velocity.z }, (Vector2) { player->velocity.x, player->velocity.z });
	if (direction_moving_dot < -0.25) // Hard stop, changing direction.
	{
		ApplyFriction(player, PLAYER_GROUND_SNAPTURN_FRICTION);
		snap_turn = TRUE;
	}
	else
	{
		ApplyFriction(player, PLAYER_GROUND_FRICTION); // Always apply some slowing.
	}

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
		Vector2 flat_facing = Vector2Normalize((Vector2) { facing_dir.x, facing_dir.z });

		float turn_modifier = 1.0f;
		float angle_modifier = Vector2Angle(dirvec, flat_facing);
		if (snap_turn || (float)fabs(angle_modifier * (float)RAD2DEG) < 9.0f)
			player->rotation = QuaternionMultiply(player->rotation, QuaternionFromAxisAngle(VEC3UP, angle_modifier)); // Snap to
		else
			player->rotation = QuaternionMultiply(player->rotation, QuaternionFromAxisAngle(VEC3UP, SIGN(angle_modifier) * turn_modifier * PLAYER_GROUND_TURN_RATE));
	}

	// Handle wall collision
	if (!player_data->disable_collision)
	{
		PlayerStandardRadialEjection(player, Vector3Scale(VEC3UP, PLAYER_COLLISION_STEP_HEIGHT), PLAYER_COLLISION_RADIUS);
		PlayerStandardRadialEjection(player, Vector3Scale(VEC3UP, PLAYER_COLLISION_MID_HEIGHT), PLAYER_COLLISION_RADIUS);
		PlayerStandardRadialEjection(player, Vector3Scale(VEC3UP, PLAYER_COLLISION_TOP_HEIGHT), PLAYER_COLLISION_RADIUS);
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

	// Get the nearest interactable actor and update the hud with it
	Vector3 ahead_pos = Vector3Add(player->position, Vector3RotateByQuaternion(VEC3FORWARD, player->rotation));
	struct Actor* nearest_actor = FINDINTERACTIONNEAREST(ahead_pos, player);

	// Interact with other actors
	player_data->current_action_button_text = ""; // Reset hud text
	if (ACTOR_EXISTS(nearest_actor) && can_accept_input)
	{
		// Check if this actor can be interacted with, if there is no set can_interact function, assume it can because it has ACTOR_FLAG_INTERACTIVE on. 
		int can_interact = ACTOR_HAS(nearest_actor, func_player_interact) && Vector3Distance(player->position, nearest_actor->position) <= ACTOR_INTERACTION_RANGE;
		if (can_interact && ACTOR_HAS(nearest_actor, func_can_interact))
			can_interact = nearest_actor->func_can_interact(nearest_actor, player);
		// Update hud
		if (can_interact)
			player_data->current_action_button_text = GetText(nearest_actor->func_interaction_text(nearest_actor, player));
		// Handle interaction button pressed
		if (can_interact && CHECK_INPUTPRESSED(input_interact))
		{
			nearest_actor->func_player_interact(nearest_actor, player);
			return;
		}
	}
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