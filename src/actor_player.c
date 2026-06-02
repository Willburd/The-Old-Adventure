#include "assets.h"
#include "actor.h"
#include "actor_factory.h"
#include "actor_player.h"

// private header
void player_preload_assets(struct Actor* actor);
void player_actor_update(struct Actor* actor);
void player_actor_draw(struct Actor* actor, float delta_time);
void player_actor_destroy(struct Actor* actor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void player_actor_init(struct Actor* actor)
{
	actor->func_load_preloadassets = player_preload_assets;
	actor->func_update = player_actor_update;
	actor->func_drawworld = player_actor_draw;
	actor->func_destroy = player_actor_destroy;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void player_preload_assets(struct Actor* actor)
{
	LoadAsset_Texture(ASSET_TEXTURES"/Objects/example.png");
}

void player_actor_update(struct Actor* actor)
{
	if (actor->position.y > 300)
		ACTOR_DESTROY(actor);
	actor->rotation = QuaternionMultiply(actor->rotation, QuaternionFromEuler(5 * DEG2RAD, 0, 0));
}

void player_actor_draw(struct Actor* actor, float delta_time)
{
	DrawTextureEx(AssetGet_Texture(ASSET_TEXTURES"/Objects/example.png"), (Vector2){ actor->position.x, actor->position.y }, QuaternionToEuler(actor->rotation).x * RAD2DEG, 1, WHITE);
}

void player_actor_destroy(struct Actor* actor)
{
	ACTOR_FACTORY(player, (Vector3) { rand() % 200, rand() % 400, 0 }, QuaternionIdentity(), Vector3One(), (Vector3) { 1, 1, 0 });
}