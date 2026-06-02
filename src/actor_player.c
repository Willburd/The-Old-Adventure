#include "tools.h"
#include "assets.h"
#include "actor.h"
#include "actor_factory.h"
#include "actor_player.h"

// private header
void player_preload_assets(struct Actor* actor);
void player_actor_update(struct Actor* actor);
void player_actor_drawworld(struct Actor* actor, float delta_time);
void player_actor_drawhud(struct Actor* actor, float delta_time);
void player_actor_destroy(struct Actor* actor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void player_actor_init(struct Actor* actor)
{
	actor->func_load_preloadassets = player_preload_assets;
	actor->func_update = player_actor_update;
	actor->func_drawworld = player_actor_drawworld;
	actor->func_drawhud = player_actor_drawhud;
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
	const boundary = 600;
	const ply_speed = 10;
	if (actor->position.x > boundary)
	{
		actor->velocity.x = -ply_speed;
		actor->velocity.y = RAND_RANGE(-ply_speed, ply_speed);
	}
	if (actor->position.y > boundary)
	{
		actor->velocity.x = RAND_RANGE(-ply_speed, ply_speed);
		actor->velocity.y = -ply_speed;
	}
	if (actor->position.x < 0)
	{
		actor->velocity.x = +ply_speed;
		actor->velocity.y = RAND_RANGE(-ply_speed, ply_speed);
	}
	if (actor->position.y < 0)
	{
		actor->velocity.x = RAND_RANGE(-ply_speed, ply_speed);
		actor->velocity.y = +ply_speed;
	}
	
	//actor->rotation = QuaternionMultiply(actor->rotation, QuaternionFromEuler(15 * DEG2RAD, 0, 0));
}

void player_actor_drawworld(struct Actor* actor, double tick_percent)
{
	DrawCube(Vector3Add(Vector3Scale(VEC3FORWARD,5.0f), Vector3Scale(ACTOR_POS_DELTA(actor, tick_percent), 0.01f)), 0.2f, 0.2f, 0.2f, RED);
}

void player_actor_drawhud(struct Actor* actor, double tick_percent)
{
	Vector3 delta_pos = ACTOR_POS_DELTA(actor, tick_percent);
	DrawTextureEx(AssetGet_Texture(PLAYER_ASSET_TEXTURE), (Vector2){ delta_pos.x, delta_pos.y }, QuaternionToEuler(ACTOR_ROT_DELTA(actor, tick_percent)).x * RAD2DEG, 1, WHITE);
}

void player_actor_destroy(struct Actor* actor)
{

}