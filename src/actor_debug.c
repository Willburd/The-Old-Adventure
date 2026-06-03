#include "tools.h"
#include "assets.h"
#include "actor.h"

// private header
void actor_debug_predrawworld(struct Actor* scene, double tick_percent);
void actor_debug_postdrawhud(struct Actor* actor, double delta_time);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void actor_debug_init(struct Actor* actor)
{
	actor->func_predrawworld = actor_debug_predrawworld;
	actor->func_postdrawhud = actor_debug_postdrawhud;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void actor_debug_predrawworld(struct Actor* scene, double tick_percent)
{
	DrawGrid(100, 1.0f);
}

void actor_debug_postdrawhud(struct Actor* actor, double tick_percent)
{
	DrawFPS(10, 10);
	DrawText(TextFormat("Actor Total: [%i]\nSeconds [%f]\nTicks [%i]\nDelta [%f]", total_actors, seconds_counter, tick_counter, tick_percent), 10, 30, 10, BLACK);
}
