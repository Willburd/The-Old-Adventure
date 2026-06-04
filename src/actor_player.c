#include "tools.h"
#include "assets.h"
#include "actor.h"

// private header
void actor_player_preload_assets(struct Actor* actor);
void actor_player_update(struct Actor* actor);
void actor_player_drawworld(struct Actor* actor, double delta_time);
void actor_player_drawhud(struct Actor* actor, double delta_time);
void actor_player_destroy(struct Actor* actor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void actor_player_init(struct Actor* actor)
{
	actor->func_preloadassets = actor_player_preload_assets;
	actor->func_update = actor_player_update;
	actor->func_drawworld = actor_player_drawworld;
	actor->func_drawhud = actor_player_drawhud;
	actor->func_destroy = actor_player_destroy;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void actor_player_preload_assets(struct Actor* actor)
{

}

void actor_player_update(struct Actor* actor)
{

}

void actor_player_drawworld(struct Actor* actor, double tick_percent)
{
	DrawCube(actor->position, 0.2f, 0.2f, 0.2f, GREEN);
}

void actor_player_drawhud(struct Actor* actor, double tick_percent)
{

}

void actor_player_destroy(struct Actor* actor)
{

}