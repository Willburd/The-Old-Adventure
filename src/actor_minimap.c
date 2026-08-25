#include "actor_factory.h"
#include "tools.h"
#include "game_draw.h"
#include "actor_scene.h"
#include "hud.h"

// Utility
#define MAP_RESOLUTION 64.0f
#define MAP_EDGE_BORDER 10.0f

// Assets
#define BLEND_MATERIAL_MAIN ASSET_MATERIALS"/Engine/color_blend.mat"

// private header
ACTOR_PRELOADASSETS(minimap);
ACTOR_POSTUPDATE(minimap);
ACTOR_DRAWHUD(minimap);
Vector2 ScaleToMap(struct Actor* actor, Vector3 pos);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(minimap)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_TEXTBOX;
	ACTOR_REGISTER_POSTUPDATE(minimap);
	ACTOR_REGISTER_DRAWHUD(minimap);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_POSTUPDATE(minimap)
{
	struct Actor* player = FINDACTORTYPE(act_player);
	if (player == NULL)
	{
		actor->actor_flags |= ACTOR_FLAG_IS_INVISIBLE;
		return;
	}
	actor->actor_flags &= ~ACTOR_FLAG_IS_INVISIBLE;

}

ACTOR_DRAWHUD(minimap)
{
	float map_left = (float)(HUD_RIGHT - (MAP_RESOLUTION + MAP_EDGE_BORDER));
	float map_top = (float)(HUD_BOTTOM - (MAP_RESOLUTION + MAP_EDGE_BORDER));
	DrawRectangleLines(map_left, map_top, MAP_RESOLUTION, MAP_RESOLUTION, WHITE);
	
	struct Actor* player = FINDACTORTYPE(act_player);
	if (player != NULL)
	{
		// Draw player position
		Vector2 pos = ScaleToMap(actor, player->position);
		DrawCircle(pos.x, pos.y, 2, YELLOW);
	}
}

Vector2 ScaleToMap(struct Actor* actor, Vector3 pos)
{
	float map_center_x = (float)(HUD_RIGHT - (MAP_RESOLUTION + MAP_EDGE_BORDER)) + (MAP_RESOLUTION * 0.5f);
	float map_center_y = (float)(HUD_BOTTOM - (MAP_RESOLUTION + MAP_EDGE_BORDER)) + (MAP_RESOLUTION * 0.5f);
	Vector2 solve_pos = (Vector2){ (actor->position.x + pos.x) * actor->scale.x, (actor->position.z + pos.z) * actor->scale.z };
	return (Vector2) { map_center_x + solve_pos.x, map_center_y + solve_pos.y };
}