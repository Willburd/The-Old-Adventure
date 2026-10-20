#include "../actor_factory.h"
#include "../tools.h"
#include "../game_draw.h"
#include "core_assets.h"
#include "../scene_entry.h"
#include "json_properties.h"

/*******************************************
				Room Swap
A spherical trigger zone that will activate a
specified room within a scene when the player
enters it. Be sure that the destination room
does not have an overlapping room swap with the
previous scene.
********************************************/

// Assets

// private header
ACTOR_JSON_INIT(roomswap);
ACTOR_TRANSPARENTDRAWWORLD(roomswap);
ACTOR_POSTUPDATE(roomswap);

typedef struct
{
	int goal_room;
	float radius;
} RoomSwapData;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(roomswap)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME;
	actor->draw_range = INFINITY;
	actor->blend_color = ColorToVector4(BLACK);
	ACTOR_REGISTER_JSON_INIT(roomswap);
	ACTOR_REGISTER_POSTUPDATE(roomswap);
	ACTOR_REGISTER_TRANSPARENTDRAWWORLD(roomswap);

	// Set data
	MALLOC_ACTOR_DATA(RoomSwapData, actor->data);
	RoomSwapData* fade_data = actor->data;
	fade_data->goal_room = 0;
	fade_data->radius = 5.0f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_JSON_INIT(roomswap)
{
	if (file_data == NULL)
		return;

	RoomSwapData* swap_data = actor->data;
	JSON_GET_INT(swap_data->goal_room, file_data, PROP_GOALROOM, 0);
	JSON_GET_FLOAT(swap_data->radius, file_data, PROP_RADIUS, 5.0f);
}

ACTOR_POSTUPDATE(roomswap)
{
	struct Actor* player = FINDACTOR_BYTYPE(act_player);
	if (!ACTOR_EXISTS(player))
		return;
	RoomSwapData* swap_data = actor->data;
	if (Vector3Distance(player->position, actor->position) > swap_data->radius)
		return;
	// If near enough, check dotproduct, if it's behind us change rooms and disable us. 
	// We should ONLY ever exist for THAT room! Don't be in a global layer!!
	if (Vector3DotProduct(Vector3RotateByQuaternion(VEC3FORWARD, actor->rotation), VEC3DIRECTION(actor->position, player->position)) > 0)
		return;
	ChangeSceneRoom(GetCurrentScene(), swap_data->goal_room, TRUE, FALSE);
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
}

ACTOR_TRANSPARENTDRAWWORLD(roomswap)
{
	if (!draw_debug_info)
		return;
	RoomSwapData* swap_data = actor->data;
	DrawCube(actor->position, 1, 1, 1, BLUE);
	DrawCube(Vector3Add(actor->position,Vector3RotateByQuaternion(VEC3FORWARD,actor->rotation)), swap_data->radius, swap_data->radius, swap_data->radius, YELLOW);
}