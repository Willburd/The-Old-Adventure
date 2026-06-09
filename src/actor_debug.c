#include "tools.h"
#include "assets.h"
#include "actor_library.h"
#include "actor_scene.h"
#include "game_draw.h"
#include "gamestate.h"

// private header
static void actor_debug_drawworld(struct Actor* scene, double tick_percent);
static void actor_debug_postdrawhud(struct Actor* actor, double delta_time);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void actor_debug_init(struct Actor* actor)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_PAUSED;
	//actor->func_drawworld = actor_debug_drawworld;
	actor->func_postdrawhud = actor_debug_postdrawhud;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void actor_debug_drawworld(struct Actor* scene, double tick_percent)
{
	if (!draw_debug_info)
		return;
	DrawGrid(100, 1.0f);
}

static void actor_debug_postdrawhud(struct Actor* actor, double tick_percent)
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

	float day_intensity = GetDayIntensity();
	float night_intensity = GetNightIntensity();
	float dawn_intensity = GetDawnIntensity();
	float dusk_intensity = GetDuskIntensity();
	float rain_intensity = 0.0f;
	DrawText(TextFormat("Actor Total: [%i]\nSeconds [%f]\nTicks [%i]\nDelta [%f]\nCurrent room: %i\n\n\nday: %f\nnight: %f\ndawn: %f\ndusk: %f\nrain: %f\n", 
		total_actors, seconds_counter, tick_counter, tick_percent, room_index, day_intensity, night_intensity, dawn_intensity, dusk_intensity, rain_intensity),
		5, 20, 4, GRAY);
}
