#include "player.h"
#include "../hud.h"
#include "inventory.h"
#include "../text_loading.h"

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
	struct Actor* camera = FINDACTOR_BYTYPE(act_camera);
	CameraData* cam_data = (CameraData*)camera->data;

	int can_accept_player_input = TRUE;
	if (cam_data->camera_mode == CAMERA_MODE_FREEMOVE)
		can_accept_player_input = FALSE;
	if (!CHECK_GAMESTATE( GAMESTATE_GAMEPLAY ))
		can_accept_player_input = FALSE;

	return can_accept_player_input;
}

int PlayerCollisionEject(struct Actor* player, Vector3 start_offset, Vector3 dirvec, float radius)
{
	Ray check_ray = {
		.position = Vector3Add(player->position, start_offset),
		.direction = dirvec
	};
	RayHitData collision = CollisionGetNearest(check_ray, radius, COL_LAYER_WORLD | COL_LAYER_MOVINGPLATFORM);
	if (!collision.ray_col.hit)
		return FALSE;
	float slope_check = Vector3DotProduct(VEC3UP, collision.ray_col.normal);
	if (slope_check >= PLAYER_FLOOR_SLOPE_DOTTHRESHOLD) // It's a floor, we're probably going up steps
		return FALSE;
	// Eject player by the remaining distance of the hit
	float remaining_dist = radius - collision.ray_col.distance;
	player->position = Vector3Subtract(player->position, Vector3Scale(dirvec, remaining_dist));
	return TRUE;
}

#define TOTAL_ANGLES 8.0f
int PlayerStandardRadialEjection(struct Actor* player, Vector3 start_offset, float radius)
{
	float angle_divisions = (360.0f / TOTAL_ANGLES) * DEG2RAD;
	int collisions = 0;
	for (int i = 0; i < TOTAL_ANGLES; i++)
	{
		Quaternion dir_angle = QuaternionFromAxisAngle(VEC3UP, angle_divisions * i);
		Quaternion player_angle = QuaternionGetFlat(player->rotation, VEC3UP);
		collisions += PlayerCollisionEject(player, start_offset, Vector3RotateByQuaternion(VEC3FORWARD, QuaternionMultiply(player_angle, dir_angle)), radius);
	}
	return collisions;
}
#undef TOTAL_ANGLES

void PlayerStandardHudDraw(struct Actor* player, double tick_percent)
{
	Texture* backing_tex = AssetGet_Texture(ASSET_TEXTURES"/Hud/HealthBack.png");
	Texture* quarter_tex = AssetGet_Texture(ASSET_TEXTURES"/Hud/HealthQuarter.png");
	Texture* half_tex = AssetGet_Texture(ASSET_TEXTURES"/Hud/HealthHalf.png");
	Texture* threequart_tex = AssetGet_Texture(ASSET_TEXTURES"/Hud/HealthThreeQuarter.png");
	Texture* full_tex = AssetGet_Texture(ASSET_TEXTURES"/Hud/HealthFull.png");
	Texture* button_tex = AssetGet_Texture(ASSET_TEXTURES"/Hud/HudButton.png");

	// Draw health
	const int heart_gap = 12;
	unsigned int heart_count = 0;
	unsigned int health_remaining = player_inventory->health;
	while (heart_count < player_inventory->max_hearts) {
		// Put on hud
		int xpos = HUD_LEFT + 5 + ((heart_count % 10) * heart_gap);
		int ypos = HUD_TOP + 5 + ((heart_count / 10)) * heart_gap;
		Vector2 pos = (Vector2){ xpos, ypos };

		// Animate the heart beating
		float draw_scale = 0.8f;
		if (health_remaining > 0 && health_remaining <= HEALTH_PER_HEART)
		{
			float pulse = (float)(tick_counter + tick_percent) * 0.03f;
			draw_scale = draw_scale + 0.09f + ((float)sin(pulse) * 0.06f);
			pos.x -= (draw_scale * 0.5f);
			pos.y -= (draw_scale * 0.5f);
		}

		// Draw segments
		DrawTextureEx(*backing_tex, pos, 0.0f, draw_scale, WHITE);
		if (health_remaining >= HEALTH_PER_HEART)
			DrawTextureEx(*full_tex, pos, 0.0f, draw_scale, WHITE);
		else if (health_remaining >= (int)((float)HEALTH_PER_HEART * 0.75f))
			DrawTextureEx(*threequart_tex, pos, 0.0f, draw_scale, WHITE);
		else if (health_remaining >= (int)((float)HEALTH_PER_HEART * 0.5f))
			DrawTextureEx(*half_tex, pos, 0.0f, draw_scale, WHITE);
		else if (health_remaining > 0)
			DrawTextureEx(*quarter_tex, pos, 0.0f, draw_scale, WHITE);
		health_remaining -= HEALTH_PER_HEART;
		heart_count++;
	}

	// Action button
	PlayerData* player_data = (PlayerData*)player->data;
	int button_start_x = HUD_RIGHT - 164;
	DrawTextureEx(*button_tex, (Vector2){ (float)(button_start_x), (float)(HUD_TOP + 5) }, 0.0f, 0.5f, GREEN);
	DrawTextureEx(*button_tex, (Vector2) { (float)(button_start_x + 35), (float)(HUD_TOP + 10) }, 0.0f, 0.5f, BLUE);
	DrawText(player_data->current_action_button_text, (float)(button_start_x + 35), (float)(HUD_TOP + 10), 12, WHITE);

	int item_start_x = button_start_x + 80;
	DrawTextureEx(*button_tex, (Vector2) { (float)(item_start_x), (float)(HUD_TOP + 10) }, 0.0f, 0.40f, YELLOW);
	DrawTextureEx(*button_tex, (Vector2) { (float)(item_start_x + 25), (float)(HUD_TOP + 20) }, 0.0f, 0.40f, YELLOW);
	DrawTextureEx(*button_tex, (Vector2) { (float)(item_start_x + 50), (float)(HUD_TOP + 10) }, 0.0f, 0.40f, YELLOW);
}

void PlayerStandardPauseActivate(struct Actor* player)
{
	if (FINDACTOR_BYTYPE(act_pause_box))
		return;
	ACTOR_FACTORY(NULL, act_pause_box, GETSCENE(player), Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero(), Vector3Zero());
}

void PlayerStandardBehavior(struct Actor* player, int can_accept_input, float max_speed, float acceleration, float slowing_friction, float stoping_friction, float snapturn_friction, float turn_rate)
{
	PlayerData* player_data = (PlayerData*)player->data;
	Vector3 move_velocity = { 0 };
	if (can_accept_input)
	{
		// Pausing
		if (CHECK_INPUTPRESSED(input_pause))
		{
			PlayerStandardPauseActivate(player);
		}

		// Handle player inputs
		Quaternion input_rotator = QuaternionFromAxisAngle(VEC3UP, Vector3GetTopDownAngle(VEC3DIRECTION(cam_main.position, player->position)));
		move_velocity = Vector3Scale(Vector3RotateByQuaternion((Vector3) { input_analog.x, 0.0f, input_analog.y }, input_rotator), acceleration);
	}
	else if (CHECK_GAMESTATE(GAMESTATE_TRANSITION | GAMESTATE_CUTSCENE))
	{
		// Cutscene movement, use the rungoal vector
		if (player_data->cutscene_run_goal.x != 0 || player_data->cutscene_run_goal.z != 0)
			move_velocity = Vector3Scale(Vector3FlatDirection(player->position, player_data->cutscene_run_goal), acceleration * player_data->cutscene_run_factor);
	}

	// Slowdown over time if not moving.
	if (Vector3Length(move_velocity) < 0.01f)
		ApplyFriction(player, stoping_friction);

	// Move as directed
	int snap_turn = FALSE;
	float direction_moving_dot = Vector2DotProduct((Vector2) { move_velocity.x, move_velocity.z }, (Vector2) { player->velocity.x, player->velocity.z });
	if (direction_moving_dot < -0.25) // Hard stop, changing direction.
	{
		ApplyFriction(player, snapturn_friction);
		snap_turn = TRUE;
	}
	else
	{
		ApplyFriction(player, slowing_friction); // Always apply some slowing.
	}

	// Accelerate up to full!
	player->velocity = Vector3Add(player->velocity, move_velocity);

	// Slow the player back down if they go over the cap speed.
	Vector2 flat_velocity = (Vector2){ player->velocity.x, player->velocity.z };
	if (Vector2Length(flat_velocity) > max_speed)
	{
		Vector2 dirvec = Vector2Scale(Vector2Normalize(flat_velocity), max_speed);
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
			player->rotation = QuaternionMultiply(player->rotation, QuaternionFromAxisAngle(VEC3UP, SIGN(angle_modifier) * turn_modifier * turn_rate));
	}
}

void PlayerStandardInteraction(struct Actor* player, int can_accept_input)
{
	PlayerData* player_data = (PlayerData*)player->data;

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

Vector3 CameraPlayerLookPos(struct Actor* camera, struct Actor* player)
{
	return Vector3Add(player->position, Vector3Scale(VEC3UP, CAMERA_PLAYER_LOOK_HEIGHT));
}