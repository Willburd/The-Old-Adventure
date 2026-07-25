#include "tools.h"
#include "assets.h"
#include "actor.h"

// private header
ACTOR_PRELOADASSETS(test);
ACTOR_UPDATE(test);
ACTOR_DRAWWORLD(test);
ACTOR_DRAWHUD(test);
ACTOR_CLEANUP(test);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(test)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME;
	ACTOR_REGISTER_PRELOADASSETS(test);
	ACTOR_REGISTER_UPDATE(test);
	ACTOR_REGISTER_DRAWWORLD(test);
	ACTOR_REGISTER_DRAWHUD(test);
	ACTOR_REGISTER_CLEANUP(test);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(test)
{

}

ACTOR_UPDATE(test)
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

ACTOR_DRAWWORLD(test)
{
	DrawCube(Vector3Add(Vector3Scale(VEC3FORWARD, 5.0f), Vector3Scale(ACTOR_POS_DELTA(actor, (float)tick_percent), 0.01f)), 0.2f, 0.2f, 0.2f, RED);
}

ACTOR_DRAWHUD(test)
{
	Vector3 delta_pos = ACTOR_POS_DELTA(actor, (float)tick_percent);
	Texture2D resolvetex = *AssetGet_Texture(ASSET_TEXTURES"/Objects/example.png");
	DrawTextureEx(resolvetex, (Vector2) { delta_pos.x, delta_pos.y }, QuaternionToEuler(ACTOR_ROT_DELTA(actor, (float)tick_percent)).x* RAD2DEG, 1, WHITE);
}

ACTOR_CLEANUP(test)
{

}