#include "tools.h"
#include "assets.h"
#include "actor_library.h"
#include "actor_scene.h"
#include "game_draw.h"

// private header
void actor_debug_drawworld(struct Actor* scene, double tick_percent);
void actor_debug_postdrawhud(struct Actor* actor, double delta_time);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void actor_debug_init(struct Actor* actor)
{
	//actor->func_drawworld = actor_debug_drawworld;
	actor->func_postdrawhud = actor_debug_postdrawhud;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void actor_debug_drawworld(struct Actor* scene, double tick_percent)
{
	if (!draw_debug_info)
		return;
	DrawGrid(100, 1.0f);
}

void actor_debug_postdrawhud(struct Actor* actor, double tick_percent)
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
	DrawText(TextFormat("Actor Total: [%i]\nSeconds [%f]\nTicks [%i]\nDelta [%f]\nCurrent room: %i\n", total_actors, seconds_counter, tick_counter, tick_percent, room_index), 5, 20, 4, GRAY);
}
