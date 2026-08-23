#include "actor_factory.h"
#include "tools.h"
#include "game_draw.h"
#include "core_assets.h"

// Assets
#define HOLE_MATERIAL_MAIN ASSET_MATERIALS"/Effects/pit_warp.mat"

// private header
ACTOR_PRELOADASSETS(roomfade);
ACTOR_TRANSPARENTDRAWWORLD(roomfade);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(roomfade)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME;
	actor->blend_color = ColorToVector4(BLACK);
	ACTOR_REGISTER_PRELOADASSETS(roomfade);
	ACTOR_REGISTER_TRANSPARENTDRAWWORLD(roomfade);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(roomfade)
{

}

ACTOR_TRANSPARENTDRAWWORLD(roomfade)
{
	if (OutOfRenderRange(actor))
		return;

	STANDARD_SHADER_MATERIAL(transition_mat, HOLE_MATERIAL_MAIN, actor);
	DrawMesh(
		AssetGet_Model(SPRITE_MODEL)->meshes[0],
		*transition_mat,
		GetMatrix(actor)
	);

	if (!draw_debug_info)
		return;
	DrawSphereWires(actor->position, actor->scale.x, 10, 10, WHITE);
}