#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "game_draw.h"
#include "collision.h"
#include "Module/core_assets.h"

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
	Asset* model_asset = AssetGetPackage(FORWARD_ARROW_MODEL);
	STANDARD_SHADER_DRAW(actor, model_asset, ASSET_MATERIALS"/Engine/example.mat", "Arrow-Material", tick_percent);
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
