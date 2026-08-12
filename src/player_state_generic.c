#include "player.h"
#include "hud.h"
#include "inventory.h"

#define PLAYER_FLOOR_SLOPE_DOTTHRESHOLD 0.6

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

int PlayerCollisionEject(struct Actor* player, Vector3 start_offset, Vector3 dirvec, float radius)
{
	Ray check_ray = {
		.position = Vector3Add(player->position, start_offset),
		.direction = dirvec
	};
	RayCollision collision = CollisionGetNearest(check_ray, radius, COL_LAYER_WORLD | COL_LAYER_MOVINGPLATFORM);
	if (!collision.hit)
		return FALSE;
	float slope_check = Vector3DotProduct(VEC3UP, collision.normal);
	if (slope_check >= PLAYER_FLOOR_SLOPE_DOTTHRESHOLD) // It's a floor, we're probably going up steps
		return FALSE;
	// Eject player by the remaining distance of the hit
	float remaining_dist = radius - collision.distance;
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
	if (FINDACTORTYPE(act_pause_box))
		return;
	ACTOR_FACTORY(NULL, act_pause_box, GETSCENE(player), Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
}