#include "game_draw.h"
#include "camera.h"
#include "globals.h"
#include "actor.h"

int draw_debug_info = FALSE;

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