#include "actor_factory.h"
#include "tools.h"
#include "game_draw.h"
#include "core_assets.h"
#include "actor_scene.h"
#include "json_properties.h"

// Assets

// private header
ACTOR_PRELOADASSETS(roomswap);
ACTOR_JSON_INIT(roomswap);
ACTOR_TRANSPARENTDRAWWORLD(roomswap);
ACTOR_POSTUPDATE(roomswap);

typedef struct
{
	int goal_room;
} RoomSwapData;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(roomswap)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME;
	actor->blend_color = ColorToVector4(BLACK);
	ACTOR_REGISTER_PRELOADASSETS(roomswap);
	ACTOR_REGISTER_JSON_INIT(roomswap);
	ACTOR_REGISTER_POSTUPDATE(roomswap);
	ACTOR_REGISTER_TRANSPARENTDRAWWORLD(roomswap);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(roomswap)
{
	// Set data
	MALLOC_ACTOR_DATA(RoomSwapData, actor->data);
	RoomSwapData* fade_data = actor->data;
	fade_data->goal_room = 0;
}

ACTOR_JSON_INIT(roomswap)
{
	if (file_data == NULL)
		return;
	RoomSwapData* swap_data = actor->data;
	if (cJSON_GetObjectItem(file_data, PROP_GOALROOM))
	{
		swap_data->goal_room = cJSON_GetObjectItem(file_data, PROP_GOALROOM)->valueint;
	}
}

ACTOR_POSTUPDATE(roomswap)
{
	struct Actor* player = FINDACTORTYPE(act_player);
	if (player == NULL)
		return;
	if (Vector3Distance(player->position, actor->position) > 5.0f)
		return;
	// If near enough, check dotproduct, if it's behind us change rooms and disable us. 
	// We should ONLY ever exist for THAT room! Don't be in a global layer!!
	if (Vector3DotProduct(Vector3RotateByQuaternion(VEC3FORWARD, actor->rotation), VEC3DIRECTION(actor->position, player->position)) > 0)
		return;
	RoomSwapData* swap_data = actor->data;
	ChangeSceneRoom(GetCurrentScene(), swap_data->goal_room, TRUE, FALSE);
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
}

ACTOR_TRANSPARENTDRAWWORLD(roomswap)
{
	if (!draw_debug_info)
		return;
	DrawCube(actor->position, 1, 1, 1, BLUE);
	DrawCube(Vector3Add(actor->position,Vector3RotateByQuaternion(VEC3FORWARD,actor->rotation)), 0.5, 0.5, 0.5, YELLOW);
}