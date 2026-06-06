#include "game_draw.h"
#include "camera.h"
#include "globals.h"
#include "actor.h"
#include "rlgl.h"

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


	////////////////////////////////////////////////////////////////////////
	// Predraw
	////////////////////////////////////////////////////////////////////////
	BeginTextureMode(render_tex_pre);
	ClearBackground(clear_background_color);
	BeginMode3D(cam_main);
	for (int i = 0; i <= current_actor_cap; i++)
	{
		rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE_MINUS_SRC_ALPHA, RL_FUNC_ADD, RL_FUNC_ADD);
		BeginBlendMode(BLEND_CUSTOM_SEPARATE);

		struct Actor* draw_actor = world_actors[i];
		if (!ACTOR_EXISTS(draw_actor))
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
	ClearBackground((Color) { 0xff, 0xff, 0xff, 0x00 });
	BeginMode3D(cam_main);
	for (int i = 0; i <= current_actor_cap; i++)
	{
		rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE_MINUS_SRC_ALPHA, RL_FUNC_ADD, RL_FUNC_ADD);
		BeginBlendMode(BLEND_CUSTOM_SEPARATE);

		struct Actor* draw_actor = world_actors[i];
		if (!ACTOR_EXISTS(draw_actor))
			continue;
		if (ACTOR_HAS(draw_actor, func_drawworld))
			draw_actor->func_drawworld(draw_actor, tick_percent);
	}
	EndMode3D();
	EndTextureMode();


	////////////////////////////////////////////////////////////////////////
	// Post draw
	////////////////////////////////////////////////////////////////////////

	BeginTextureMode(render_tex_post);
	ClearBackground((Color) { 0xff, 0xff, 0xff, 0x00 });
	BeginMode3D(cam_main);
	for (int i = 0; i <= current_actor_cap; i++)
	{
		rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE_MINUS_SRC_ALPHA, RL_FUNC_ADD, RL_FUNC_ADD);
		BeginBlendMode(BLEND_CUSTOM_SEPARATE);

		struct Actor* draw_actor = world_actors[i];
		if (!ACTOR_EXISTS(draw_actor))
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
	ClearBackground((Color) { 0xff, 0xff, 0xff, 0x00 });
	BeginMode2D(cam_hud);
	{
		rlSetBlendFactorsSeparate(RL_SRC_ALPHA, RL_ONE_MINUS_SRC_ALPHA, RL_ONE, RL_ONE_MINUS_SRC_ALPHA, RL_FUNC_ADD, RL_FUNC_ADD);
		BeginBlendMode(BLEND_CUSTOM_SEPARATE);

		for (int i = 0; i <= current_actor_cap; i++)
		{
			struct Actor* draw_actor = world_actors[i];
			if (!ACTOR_EXISTS(draw_actor))
				continue;
			if (ACTOR_HAS(draw_actor, func_predrawhud))
				draw_actor->func_predrawhud(draw_actor, tick_percent);
		}
		for (int i = 0; i <= current_actor_cap; i++)
		{
			struct Actor* draw_actor = world_actors[i];
			if (!ACTOR_EXISTS(draw_actor))
				continue;
			if (ACTOR_HAS(draw_actor, func_drawhud))
				draw_actor->func_drawhud(draw_actor, tick_percent);
		}
		for (int i = 0; i <= current_actor_cap; i++)
		{
			struct Actor* draw_actor = world_actors[i];
			if (!ACTOR_EXISTS(draw_actor))
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
	fog_color = (Vector4){ (float)col.r / 255.0f, (float)col.g / 255.0f, (float)col.b / 255.0f, (float)col.a / 255.0f };
}

void lighting_append_light(Vector3 pos, float radius, Color col, float influence)
{
	if (light_count >= MAX_LIGHTS) // Find furthest light and replace it
	{
		int furthest_index = 0;
		float furthest_distance = 0;
		Vector3 cam_pos = cam_main.position;
		for (int i = 0; i < MAX_LIGHTS; i += 1)
		{

		}
		return;
	}
	world_light_positions[light_count] = (Vector4){ pos.x, pos.y, pos.z, radius };
	world_light_colors[light_count] = (Vector4){ (float)col.r / 255.0f, (float)col.g / 255.0f, (float)col.b / 255.0f, influence };
	light_count++;
}

void shader_update_fog(Shader shader)
{
	int fog_loc = GetShaderLocation(shader, "uFogColor");
	SetShaderValue(shader, fog_loc, &fog_color, SHADER_UNIFORM_VEC4);
	fog_loc = GetShaderLocation(shader, "uFogPower");
	SetShaderValue(shader, fog_loc, &fog_power, SHADER_UNIFORM_FLOAT);
	fog_loc = GetShaderLocation(shader, "uFogDistance");
	SetShaderValue(shader, fog_loc, &fog_distance, SHADER_UNIFORM_FLOAT);
}

void shader_update_camera_pos(Shader shader)
{
	float camera_pos[3] = { cam_main.position.x, cam_main.position.y, cam_main.position.z };
	SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], camera_pos, SHADER_UNIFORM_VEC3);
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