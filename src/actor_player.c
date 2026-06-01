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
	actor->position.x += 1;
	actor->position.y += 1;

}

void player_actor_draw(struct Actor* actor, float delta_time)
{
	Vector2 vec = { actor->position.x, actor->position.y };
	Texture2D tex = AssetGet_Texture(ASSET_TEXTURES"/Objects/example.png");
	DrawTexture(tex,vec.x,vec.y, WHITE);
}

void player_actor_destroy(struct Actor* actor)
{

}