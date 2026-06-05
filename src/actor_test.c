#include "tools.h"
#include "assets.h"
#include "actor.h"

// private header
void actor_test_preload_assets(struct Actor* actor);
void actor_test_update(struct Actor* actor);
void actor_test_drawworld(struct Actor* actor, double delta_time);
void actor_test_drawhud(struct Actor* actor, double delta_time);
void actor_test_destroy(struct Actor* actor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void actor_test_init(struct Actor* actor)
{
	actor->func_preloadassets = actor_test_preload_assets;
	actor->func_update = actor_test_update;
	actor->func_drawworld = actor_test_drawworld;
	actor->func_drawhud = actor_test_drawhud;
	actor->func_destroy = actor_test_destroy;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TEST_ASSET_TEXTURE ASSET_TEXTURES"/Objects/example.png"

void actor_test_preload_assets(struct Actor* actor)
{
	LoadAsset_Texture(TEST_ASSET_TEXTURE, FALSE, NULL);
}

void actor_test_update(struct Actor* actor)
{
	const boundary = 600;
	const ply_speed = 10;
	if (actor->position.x > boundary)
	{
		actor->velocity.x = (float)-ply_speed;
		actor->velocity.y = (float)RAND_RANGE(-ply_speed, ply_speed);
	}
	if (actor->position.y > boundary)
	{
		actor->velocity.x = (float)RAND_RANGE(-ply_speed, ply_speed);
		actor->velocity.y = (float)-ply_speed;
	}
	if (actor->position.x < 0)
	{
		actor->velocity.x = (float)+ply_speed;
		actor->velocity.y = (float)RAND_RANGE(-ply_speed, ply_speed);
	}
	if (actor->position.y < 0)
	{
		actor->velocity.x = (float)RAND_RANGE(-ply_speed, ply_speed);
		actor->velocity.y = (float)+ply_speed;
	}

	//actor->rotation = QuaternionMultiply(actor->rotation, QuaternionFromEuler(15 * DEG2RAD, 0, 0));
}

void actor_test_drawworld(struct Actor* actor, double tick_percent)
{
	DrawCube(Vector3Add(Vector3Scale(VEC3FORWARD, 5.0f), Vector3Scale(ACTOR_POS_DELTA(actor, (float)tick_percent), 0.01f)), 0.2f, 0.2f, 0.2f, RED);
}

void actor_test_drawhud(struct Actor* actor, double tick_percent)
{
	Vector3 delta_pos = ACTOR_POS_DELTA(actor, (float)tick_percent);
	Texture2D resolvetex = *AssetGet_Texture(TEST_ASSET_TEXTURE);
	DrawTextureEx(resolvetex, (Vector2) { delta_pos.x, delta_pos.y }, QuaternionToEuler(ACTOR_ROT_DELTA(actor, (float)tick_percent)).x* RAD2DEG, 1, WHITE);
}

void actor_test_destroy(struct Actor* actor)
{

}