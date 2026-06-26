#include "game_draw.h"
#include "camera.h"
#include "globals.h"
#include "actor.h"
#include "rlgl.h"
#include "tools.h"
#include "world_state.h"

int draw_debug_info = FALSE;

int light_count = 0;
Vector4 world_light_positions[MAX_LIGHTS] = { 0 };
Vector4 world_light_colors[MAX_LIGHTS] = { 0 };

Color clear_background_color;

RenderTexture render_tex_pre = { 0 };
RenderTexture render_tex_main = { 0 };
RenderTexture render_tex_post = { 0 };
RenderTexture render_tex_hud = { 0 };

void game_draw(double tick_percent)
{
	// Recalculate render size
	float window_aspect = (float)screenWidth / (float)screenHeight;
	renderWidth = (int)((float)renderHeight * window_aspect);

	int want_resize = FALSE;
	if (IsRenderTextureValid(render_tex_pre))
	{
		// Check if we have a new texture size
		if (render_tex_pre.texture.width != renderWidth || render_tex_pre.texture.height != renderHeight)
			want_resize = TRUE;
	}
	else
	{
		// Invalid tex
		want_resize = TRUE;
	}
	
	if (want_resize)
	{
		// Unload previous
		if (IsRenderTextureValid(render_tex_pre))
			UnloadRenderTexture(render_tex_pre);
		if (IsRenderTextureValid(render_tex_main))
			UnloadRenderTexture(render_tex_main);
		if (IsRenderTextureValid(render_tex_post))
			UnloadRenderTexture(render_tex_post);
		if (IsRenderTextureValid(render_tex_hud))
			UnloadRenderTexture(render_tex_hud);
		// Create size
		render_tex_pre = LoadRenderTexture(renderWidth, renderHeight);
		render_tex_main = LoadRenderTexture(renderWidth, renderHeight);
		render_tex_post = LoadRenderTexture(renderWidth, renderHeight);
		render_tex_hud = LoadRenderTexture(renderWidth, renderHeight);
	}

	// Clear color affects fog
	Color draw_clear_col = Vector3ToColor(fog_color, 0.0f);

	////////////////////////////////////////////////////////////////////////
	// Predraw
	////////////////////////////////////////////////////////////////////////
	BeginTextureMode(render_tex_pre);
	ClearBackground(clear_background_color);
	BeginMode3D(cam_main);
	// Prep shaders
	rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE_MINUS_SRC_ALPHA, RL_FUNC_ADD, RL_FUNC_ADD);
	BeginBlendMode(BLEND_CUSTOM_SEPARATE);
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* draw_actor = world_actors[i];
		if (!ACTOR_EXISTS(draw_actor))
			continue;
		if (draw_actor->actor_flags & ACTOR_FLAG_IS_INVISIBLE)
			continue;
		if (ACTOR_HAS(draw_actor, func_predrawworld))
			draw_actor->func_predrawworld(draw_actor, tick_percent);
	}
	EndMode3D();
	EndTextureMode();


	////////////////////////////////////////////////////////////////////////
	// Main draw
	////////////////////////////////////////////////////////////////////////

	BeginTextureMode(render_tex_main);
	ClearBackground(draw_clear_col);
	BeginMode3D(cam_main);
	rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE_MINUS_SRC_ALPHA, RL_FUNC_ADD, RL_FUNC_ADD);
	BeginBlendMode(BLEND_CUSTOM_SEPARATE);
	// Base pass
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* draw_actor = world_actors[i];
		if (!ACTOR_EXISTS(draw_actor))
			continue;
		if (draw_actor->actor_flags & ACTOR_FLAG_IS_INVISIBLE)
			continue;
		if (ACTOR_HAS(draw_actor, func_drawworld))
			draw_actor->func_drawworld(draw_actor, tick_percent);
	}
	// Transparent pass
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* draw_actor = world_actors[i];
		if (!ACTOR_EXISTS(draw_actor))
			continue;
		if (draw_actor->actor_flags & ACTOR_FLAG_IS_INVISIBLE)
			continue;
		if (ACTOR_HAS(draw_actor, func_transparentdrawworld))
			draw_actor->func_transparentdrawworld(draw_actor, tick_percent);
	}
	EndMode3D();
	EndTextureMode();


	////////////////////////////////////////////////////////////////////////
	// Post draw
	////////////////////////////////////////////////////////////////////////

	BeginTextureMode(render_tex_post);
	ClearBackground(draw_clear_col);
	BeginMode3D(cam_main);
	rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE_MINUS_SRC_ALPHA, RL_FUNC_ADD, RL_FUNC_ADD);
	BeginBlendMode(BLEND_CUSTOM_SEPARATE);
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* draw_actor = world_actors[i];
		if (!ACTOR_EXISTS(draw_actor))
			continue;
		if (draw_actor->actor_flags & ACTOR_FLAG_IS_INVISIBLE)
			continue;
		if (ACTOR_HAS(draw_actor, func_postdrawworld))
			draw_actor->func_postdrawworld(draw_actor, tick_percent);
	}
	EndMode3D();
	EndTextureMode();


	////////////////////////////////////////////////////////////////////////
	// Hud drawing
	////////////////////////////////////////////////////////////////////////

	Vector2 org = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
	Rectangle src = (Rectangle){ 0, 0, (float)renderWidth, (float)-renderHeight };
	Rectangle dest = (Rectangle){ screenWidth / 2.0f, screenHeight / 2.0f, (float)screenWidth, (float)screenHeight };

	BeginTextureMode(render_tex_hud);
	ClearBackground((Color) {20,20,20,0}); // Slightly gray
	BeginMode2D(cam_hud);
	{
		rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE_MINUS_SRC_ALPHA, RL_FUNC_ADD, RL_FUNC_ADD);
		BeginBlendMode(BLEND_CUSTOM_SEPARATE);
		// predraw hud
		for (int i = 0; i <= current_actor_cap; i++)
		{
			struct Actor* draw_actor = world_actors[i];
			if (!ACTOR_EXISTS(draw_actor))
				continue;
			if (draw_actor->actor_flags & ACTOR_FLAG_IS_INVISIBLE)
				continue;
			if (ACTOR_HAS(draw_actor, func_predrawhud))
				draw_actor->func_predrawhud(draw_actor, tick_percent);
		}
		// draw hud
		for (int i = 0; i <= current_actor_cap; i++)
		{
			struct Actor* draw_actor = world_actors[i];
			if (!ACTOR_EXISTS(draw_actor))
				continue;
			if (draw_actor->actor_flags & ACTOR_FLAG_IS_INVISIBLE)
				continue;
			if (ACTOR_HAS(draw_actor, func_drawhud))
				draw_actor->func_drawhud(draw_actor, tick_percent);
		}
		// postdraw hud
		for (int i = 0; i <= current_actor_cap; i++)
		{
			struct Actor* draw_actor = world_actors[i];
			if (!ACTOR_EXISTS(draw_actor))
				continue;
			if (draw_actor->actor_flags & ACTOR_FLAG_IS_INVISIBLE)
				continue;
			if (ACTOR_HAS(draw_actor, func_postdrawhud))
				draw_actor->func_postdrawhud(draw_actor, tick_percent);
		}
	}
	EndMode2D();
	EndTextureMode();

	BeginDrawing();
	BeginMode2D(cam_hud);
	DrawTexturePro(render_tex_pre.texture, src, dest, org, 0, WHITE);
	DrawTexturePro(render_tex_main.texture, src, dest, org, 0, WHITE);
	DrawTexturePro(render_tex_post.texture, src, dest, org, 0, WHITE);
	DrawTexturePro(render_tex_hud.texture, src, dest, org, 0, WHITE);
	EndMode2D();
	EndDrawing();
}

void fog_set(Color col, float power, float dist)
{
	fog_distance = dist;
	fog_power = power;
	fog_color = (Vector3){ (float)col.r / 255.0f, (float)col.g / 255.0f, (float)col.b / 255.0f };
}

// Lights are updated each frame and do not persist between them. Lights need to be "appended" to the light list each update to render. This is easier than juggling light references when wanting to animate lights.
void lighting_append_light(Vector3 pos, float radius, Color col, float influence)
{
	int write_index = light_count;
	if (light_count >= MAX_LIGHTS) // Find furthest light and replace it
	{
		int weakest_index = 0;
		float weakest_influence = INFINITY;
		Vector3 cam_pos = cam_main.position;
		for (int i = 0; i < MAX_LIGHTS; i += 1)
		{
			// Radius multiplied by alpha
			float influence = world_light_positions[i].w * world_light_colors[i].w;
			// Check distance to camera by radius of light, global lights don't do this check.
			if (world_light_positions[i].w < LIGHT_WORLD_RANGE)
			{
				float dist = Clamp(
								Vector3Distance(cam_main.position, (Vector3) { world_light_positions[i].x, world_light_positions[i].y, world_light_positions[i].z }) / world_light_positions[i].w, 
								0, 
								1
							);
				influence *= dist;
			}
			if (influence >= weakest_influence)
				continue;
			// Weaker than the weakest so far
			weakest_influence = influence;
			weakest_index = i;
		}
		write_index = weakest_index;
	}
	// Write the light to the slot decided
	world_light_positions[write_index] = (Vector4){ pos.x, pos.y, pos.z, radius };
	world_light_colors[write_index] = (Vector4){ (float)col.r / 255.0f, (float)col.g / 255.0f, (float)col.b / 255.0f, influence };
	if (++light_count > MAX_LIGHTS)
		light_count = MAX_LIGHTS;
}

void shader_update_fog(Shader shader)
{
	int fog_loc = GetShaderLocation(shader, "uFogColor");
	SetShaderValue(shader, fog_loc, &fog_color, SHADER_UNIFORM_VEC3);
	fog_loc = GetShaderLocation(shader, "uFogPower");
	SetShaderValue(shader, fog_loc, &fog_power, SHADER_UNIFORM_FLOAT);
	fog_loc = GetShaderLocation(shader, "uFogDistance");
	SetShaderValue(shader, fog_loc, &fog_distance, SHADER_UNIFORM_FLOAT);
}

void shader_update_lights(Shader shader)
{
	// Pass number of lights
	int light_loc = GetShaderLocation(shader, "uLightCount");
	if (light_loc < 0)
		return;
	SetShaderValue(shader, light_loc, &light_count, SHADER_UNIFORM_INT);

	light_loc = GetShaderLocation(shader, "uLightPositions");
	SetShaderValueV(shader, light_loc, &world_light_positions, SHADER_UNIFORM_VEC4, light_count);
	light_loc = GetShaderLocation(shader, "uLightColors");
	SetShaderValueV(shader, light_loc, &world_light_colors, SHADER_UNIFORM_VEC4, light_count);
}