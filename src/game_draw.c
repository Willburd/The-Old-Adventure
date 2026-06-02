#include "game_draw.h"
#include "camera.h"
#include "globals.h"
#include "actor.h"

void game_draw(double tick_percent)
{

    BeginDrawing();
    ClearBackground(SKYBLUE);

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

	BeginMode2D(cam_hud);
		for (int i = 0; i <= current_actor_cap; i++)
		{
			struct Actor* draw_actor = world_actors[i];
			if (!ACTOR_EXISTS(draw_actor))
				continue;
			if (ACTOR_HAS(draw_actor, func_drawhud))
				draw_actor->func_drawhud(draw_actor, tick_percent);
		}
	EndMode2D();

	DrawFPS(10, 10);
	DrawText(TextFormat("Actor Total: [%i]\nSeconds [%f]\nTicks [%i]\nDelta [%f]", total_actors, seconds_counter, tick_counter, tick_percent), 10, 30, 10, BLACK);
    EndDrawing();
}