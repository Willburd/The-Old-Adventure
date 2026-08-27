#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "game_draw.h"
#include "collision.h"

// private header
ACTOR_DRAWWORLD(debug);
ACTOR_POSTDRAWHUD(debug);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(debug)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_PAUSED;
	ACTOR_REGISTER_DRAWWORLD(debug);
	ACTOR_REGISTER_POSTDRAWHUD(debug);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_DRAWWORLD(debug)
{
	if (!draw_debug_info)
		return;
	DrawGrid(100, 10.0f);

	DrawCube(VEC3FORWARD, 1.0f, 1.0f, 1.0f, BLUE);
	DrawCube(VEC3BACKWARD, 0.1f, 0.1f, 0.1f, BLUE);
	DrawCube(VEC3LEFT, 1.0f, 1.0f, 1.0f, GREEN);
	DrawCube(VEC3RIGHT, 0.1f, 0.1f, 0.1f, GREEN);

	DrawCube(Vector3RotateByQuaternion(VEC3FORWARD, QuaternionFromAxisAngle(VEC3UP, 0.0f)), 0.25f, 2.5f, 0.25f, RED); // Should be forward
	DrawCube(Vector3RotateByQuaternion(VEC3FORWARD, QuaternionFromAxisAngle(VEC3UP, 90.0f * DEG2RAD)), 0.25f, 2.5f, 0.25f, YELLOW); // Should be left
}

ACTOR_POSTDRAWHUD(debug)
{
	if (!draw_debug_info)
		return;
	struct Actor* scene = GetCurrentScene();
	int room_index = -1;
	if (scene)
		room_index = scene->current_room_index;
	DrawFPS(5, 5);
	DrawText(TextFormat("[act: %i] [col: %i] [lig: %i]", current_actor_cap, GetColliderCount(), GetLightCount()), 5, 25, 4, WHITE);
}
