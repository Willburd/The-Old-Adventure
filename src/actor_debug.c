#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "game_draw.h"
#include "gamestate.h"

// private header
ACTOR_DRAWWORLD(debug);
ACTOR_POSTDRAWHUD(debug);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(debug)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_PAUSED;
	ACTOR_REGISTER_DRAWWORLD(debug);
	ACTOR_REGISTER_POSTDRAWHUD(debug);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_DRAWWORLD(debug)
{
	if (!draw_debug_info)
		return;
	DrawGrid(100, 10.0f);

	DrawCube(VEC3FORWARD, 1.0f, 1.0f, 1.0f, BLUE);
	DrawCube(VEC3BACKWARD, 0.1f, 0.1f, 0.1f, BLUE);
	DrawCube(VEC3LEFT, 1.0f, 1.0f, 1.0f, GREEN);
	DrawCube(VEC3RIGHT, 0.1f, 0.1f, 0.1f, GREEN);

	DrawCube(Vector3RotateByQuaternion(VEC3FORWARD, QuaternionFromAxisAngle(VEC3UP, 0.0f)), 0.25f, 2.5f, 0.25f, RED); // Should be forward
	DrawCube(Vector3RotateByQuaternion(VEC3FORWARD, QuaternionFromAxisAngle(VEC3UP, 90.0f * DEG2RAD)), 0.25f, 2.5f, 0.25f, YELLOW); // Should be left
}

ACTOR_POSTDRAWHUD(debug)
{
	if (!draw_debug_info)
		return;
	struct Actor* player = FINDACTORTYPE(act_player);
	struct Actor* scene = scene = GETSCENE(player);
	int room_index = -1;
	if (scene)
	{
		SceneData* scene_data = scene->data;
		room_index = scene_data->active_room;
	}
	DrawFPS(5, 5);
	/*
	float day_intensity = GetDayIntensity();
	float night_intensity = GetNightIntensity();
	float dawn_intensity = GetDawnIntensity();
	float dusk_intensity = GetDuskIntensity();
	float rain_intensity = 0.0f;
	DrawText(TextFormat("Actor Total: [%i]\nSeconds [%f]\nTicks [%i]\nDelta [%f]\nCurrent room: %i\n\n\nday: %f\nnight: %f\ndawn: %f\ndusk: %f\nrain: %f\n", 
		total_actors, seconds_counter, tick_counter, tick_percent, room_index, day_intensity, night_intensity, dawn_intensity, dusk_intensity, rain_intensity),
		5, 20, 4, GRAY);
	*/

	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* draw_actor = world_actors[i];
		if (!ACTOR_EXISTS(draw_actor))
			continue;
		DrawText(TextFormat("[%i] %s", i, actor_name(draw_actor->actor_type)), 5, 25 + (i*12), 4, WHITE);
	}
}
