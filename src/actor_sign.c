#include "core_assets.h"
#include "actor.h"
#include "tools.h"

// private header
static void actor_sign_preload_assets(struct Actor* actor);
static void actor_sign_interaction_text(struct Actor* actor, struct Actor* player);
static void actor_sign_player_interact(struct Actor* actor, struct Actor* player);
static void actor_sign_drawworld(struct Actor* actor, double tick_percent);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void actor_sign_init(struct Actor* actor)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_INTERACTIVE;
	actor->func_preloadassets = actor_sign_preload_assets;
	actor->func_drawworld = actor_sign_drawworld;
	actor->func_player_interact = actor_sign_player_interact;
	actor->func_interaction_text = actor_sign_interaction_text;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void actor_sign_preload_assets(struct Actor* actor)
{

}

static void actor_sign_interaction_text(struct Actor* actor, struct Actor* player)
{
	return TEXT_ACTIONBUTTON_READ;
}

static void actor_sign_player_interact(struct Actor* actor, struct Actor* player)
{
	printf("Interaction!");
}

static void actor_sign_drawworld(struct Actor* actor, double tick_percent)
{
	DrawSphereWires(actor->position, ACTOR_INTERACTION_RANGE, 5, 5, WHITE);
}