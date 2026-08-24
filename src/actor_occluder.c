#include "actor_factory.h"
#include "tools.h"
#include "game_draw.h"
#include "core_assets.h"
#include "actor_scene.h"
#include "json_properties.h"

// Utility
#define FADE_LIMIT actor->scale.z

// Assets
#define DARKNESS_MATERIAL_MAIN ASSET_MATERIALS"/Engine/black.mat"

// private header
ACTOR_PRELOADASSETS(occluder);
ACTOR_TRANSPARENTDRAWWORLD(occluder);
ACTOR_POSTUPDATE(occluder);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(occluder)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME;
	actor->blend_color = ColorToVector4(BLACK);
	ACTOR_REGISTER_POSTUPDATE(occluder);
	ACTOR_REGISTER_TRANSPARENTDRAWWORLD(occluder);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_POSTUPDATE(occluder)
{
	struct Actor* player = FINDACTORTYPE(act_player);
	if (player == NULL)
		return;
	if (Vector3Distance(player->position, actor->position) > FADE_LIMIT)
		return;
	if (Vector3DotProduct(Vector3RotateByQuaternion(VEC3FORWARD, actor->rotation), VEC3DIRECTION(actor->position, player->position)) > 0)
		return;
}

ACTOR_TRANSPARENTDRAWWORLD(occluder)
{
	STANDARD_SHADER_MATERIAL(transition_mat, DARKNESS_MATERIAL_MAIN, actor);
	DrawMesh(
		AssetGet_Model(SPRITE_MODEL)->meshes[0],
		*transition_mat,
		GetMatrix(actor)
	);

	if (!draw_debug_info)
		return;
	DrawSphereWires(actor->position, FADE_LIMIT, 10, 10, BLUE);
}