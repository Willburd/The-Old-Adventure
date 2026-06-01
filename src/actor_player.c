#include "actor.h"
#include "actor_factory.h"
#include "actor_player.h"

// private header
void player_actor_update(struct Actor* actor);
void player_actor_draw(struct Actor* actor, float delta_time);
void player_actor_destroy(struct Actor* actor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void player_actor_init(struct Actor* actor)
{
	actor->func_update = player_actor_update;
	actor->func_drawworld = player_actor_draw;
	actor->func_destroy = player_actor_destroy;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void player_actor_update(struct Actor* actor)
{
	actor->position.x += 50;
	actor->position.y += 50;

	if (actor->position.y > 100)
	{
		ACTOR_DESTROY(actor);
		return;
	}
}

void player_actor_draw(struct Actor* actor, float delta_time)
{
	Vector2 vec = { actor->position.x, actor->position.y };
	DrawCircleV(vec, 50, MAROON);
}

void player_actor_destroy(struct Actor* actor)
{
	ACTOR_FACTORY(player, (Vector3) { 0, 0, 0 }, (Vector3) { 0, 0, 0 });
}