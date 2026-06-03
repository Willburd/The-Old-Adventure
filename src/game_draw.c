#include "game_draw.h"
#include "camera.h"
#include "globals.h"
#include "actor.h"

Color clear_background_color;

void game_draw(double tick_percent)
{
    BeginDrawing();
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
		for (int i = 0; i <= current_actor_cap; i++)
		{
			struct Actor* draw_actor = world_actors[i];
			if (!ACTOR_EXISTS(draw_actor))
				continue;
			if (ACTOR_HAS(draw_actor, func_drawworld))
				draw_actor->func_drawworld(draw_actor, tick_percent);
		}
		for (int i = 0; i <= current_actor_cap; i++)
		{
			struct Actor* draw_actor = world_actors[i];
			if (!ACTOR_EXISTS(draw_actor))
				continue;
			if (ACTOR_HAS(draw_actor, func_postdrawworld))
				draw_actor->func_postdrawworld(draw_actor, tick_percent);
		}
	EndMode3D();

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

    EndDrawing();
}