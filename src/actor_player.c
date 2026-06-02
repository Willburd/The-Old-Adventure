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

#define PLAYER_ASSET_TEXTURE ASSET_TEXTURES"/Objects/example.png"

void player_preload_assets(struct Actor* actor)
{
	LoadAsset_Texture(PLAYER_ASSET_TEXTURE);
}

void player_actor_update(struct Actor* actor)
{
	if (actor->position.y > 300)
		ACTOR_DESTROY(actor);
	actor->rotation = QuaternionMultiply(actor->rotation, QuaternionFromEuler(15 * DEG2RAD, 0, 0));
}

void player_actor_draw(struct Actor* actor, float tick_percent)
{
	Vector3 delta_pos = ACTOR_POS_DELTA(actor, tick_percent);
	DrawTextureEx(AssetGet_Texture(PLAYER_ASSET_TEXTURE), (Vector2){ delta_pos.x, delta_pos.y }, QuaternionToEuler(ACTOR_ROT_DELTA(actor, tick_percent)).x * RAD2DEG, 1, WHITE);
}

void player_actor_destroy(struct Actor* actor)
{
	ACTOR_FACTORY(player, (Vector3) { rand() % 200, rand() % 400, 0 }, QuaternionIdentity(), Vector3One(), (Vector3) { 1, 1, 0 });
}