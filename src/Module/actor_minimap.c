#include "../actor_factory.h"
#include "../tools.h"
#include "../game_draw.h"
#include "../scene_entry.h"
#include "../game_state.h"
#include "../hud.h"
#include "actor_minimap.h"

/*******************************************
				Minimap
Displays a texture in the bottom right of the
screen that shows the player's position.
This actor's scale and position act as offset
and scale factor to the player's position.
Allowing any scale of minimap to room size.

Easiest method to make a map is to place POI
objects in the corners of a room, then scale 
the minimap actor until a desired shape is 
reached. Draw the minimap and remove the POIS.

Minimap textures can be any size, they will be
scaled to match the correct scale on the hud.
********************************************/

// Utility
#define MAP_RESOLUTION 64.0f
#define MAP_EDGE_BORDER 10.0f

// Assets
#define ARROW_TEXTURE ASSET_TEXTURES"/Minimaps/Arrow.png"
#define MAP_TEXTURE_PATH TextFormat(ASSET_TEXTURES"/Minimaps/%s_%i.png", scene->actor_type_name, scene->current_room_index)

// private header
ACTOR_PRELOADASSETS(minimap);
ACTOR_POSTUPDATE(minimap);
Vector2 ScaleToMap(struct Actor* actor, Vector3 pos);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(minimap)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_TEXTBOX;
	actor->draw_range = INFINITY;
	ACTOR_REGISTER_PRELOADASSETS(minimap);
	ACTOR_REGISTER_POSTUPDATE(minimap);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(minimap)
{
 	struct Actor* scene = GetCurrentScene();
	LoadAsset_Texture(MAP_TEXTURE_PATH, FALSE, NULL);
	LoadAsset_Texture(ARROW_TEXTURE, FALSE, NULL);
}

ACTOR_POSTUPDATE(minimap)
{
	struct Actor* player = FINDACTOR_BYTYPE(act_player);
	if (!ACTOR_EXISTS(player))
	{
		actor->actor_flags |= ACTOR_FLAG_IS_INVISIBLE;
		return;
	}
	actor->actor_flags &= ~ACTOR_FLAG_IS_INVISIBLE;

}

void DrawMinimapActor(struct Actor* actor, double tick_percent)
{
	if (!CHECK_GAMESTATE(GAMESTATE_GAMEPLAY))
		return;

	float map_left = (float)(HUD_RIGHT - (MAP_RESOLUTION + MAP_EDGE_BORDER));
	float map_top = (float)(HUD_BOTTOM - (MAP_RESOLUTION + MAP_EDGE_BORDER));
	struct Actor* scene = GetCurrentScene();
	if (!AssetExists(MAP_TEXTURE_PATH))
	{
		// Invalid maps just show a rectangle
		DrawRectangleLines(map_left, map_top, MAP_RESOLUTION, MAP_RESOLUTION, GREEN);
	}
	else
	{
		// Find and load the texture for the room
		Texture* tex = AssetGet_Texture(MAP_TEXTURE_PATH);
		float scale_div = MAP_RESOLUTION / tex->width;
		DrawTextureEx(*tex, (Vector2) { map_left, map_top }, 0.0f, scale_div, GREEN);
	}

	struct Actor* player = FINDACTOR_BYTYPE(act_player);
	if (ACTOR_EXISTS(player))
	{
		// Draw player position
		Texture* tex = AssetGet_Texture(ARROW_TEXTURE);
		Vector2 pos = ScaleToMap(actor, player->position);
		DrawPivotTexture(*tex, pos, (Vector2) { 4, 4 }, QuaternionGetTopDownAngle(QuaternionLerp(player->last_rotation, player->rotation, (float)tick_percent)), 1.0f, YELLOW);
	}

	{	// Draw POIs
		struct Actor* found_pois[32] = { NULL };
		int found_count = FINDACTORGROUP_BYTYPE(found_pois, 32, act_mappoi);
		if (found_count > 0)
		{
			for (int i = 0; i < found_count; i++)
			{
				struct Actor* poi = found_pois[i];
				if (poi == NULL)
					continue;
				Vector2 pos = ScaleToMap(actor, Vector3Lerp(poi->last_position, poi->position, (float)tick_percent));
				DrawCircle(pos.x, pos.y, 1, WHITE);
			}
		}
	}
}

Vector2 ScaleToMap(struct Actor* actor, Vector3 pos)
{
	float map_center_x = (float)(HUD_RIGHT - (MAP_RESOLUTION + MAP_EDGE_BORDER)) + (MAP_RESOLUTION * 0.5f);
	float map_center_y = (float)(HUD_BOTTOM - (MAP_RESOLUTION + MAP_EDGE_BORDER)) + (MAP_RESOLUTION * 0.5f);
	Vector2 solve_pos = (Vector2){ (actor->position.x + pos.x) * actor->scale.x, (actor->position.z + pos.z) * actor->scale.z };
	return (Vector2) { map_center_x + solve_pos.x, map_center_y + solve_pos.y };
}