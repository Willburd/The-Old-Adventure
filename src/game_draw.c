#include "raylib.h"
#include "globals.h"
#include "actor.h"

void game_draw(float tick_percent)
{

    BeginDrawing();

    ClearBackground(LIGHTGRAY);

	

    DrawFPS(10, 10);


	//BeginMode3D();
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* draw_actor = world_actors[i];
		if (!ACTOR_EXISTS(draw_actor))
			continue;
		if (ACTOR_HAS(draw_actor, func_drawworld))
			draw_actor->func_drawworld(draw_actor, tick_percent);
	}
	//EndMode3D();

	//BeginMode2D();
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* draw_actor = world_actors[i];
		if (!ACTOR_EXISTS(draw_actor))
			continue;
		if (ACTOR_HAS(draw_actor, func_drawhud))
			draw_actor->func_drawhud(draw_actor, tick_percent);
	}
	//EndMode2D();

	DrawText(TextFormat("Actor Total: [%i]\nSeconds [%f]\nTicks [%i]\nDelta [%f]", total_actors, seconds_counter, tick_counter, tick_percent), 10, 30, 10, BLACK);
    EndDrawing();
}