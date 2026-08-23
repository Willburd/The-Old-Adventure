#include "actor_factory.h"
#include "tools.h"
#include "game_draw.h"
#include "core_assets.h"
#include "actor_scene.h"

// Utility
#define FADE_LIMIT actor->scale.z

// Assets
#define DARKNESS_MATERIAL_MAIN ASSET_MATERIALS"/Engine/black.mat"

// private header
ACTOR_PRELOADASSETS(roomfade);
ACTOR_TRANSPARENTDRAWWORLD(roomfade);
ACTOR_POSTUPDATE(roomfade);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(roomfade)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME;
	actor->blend_color = ColorToVector4(BLACK);
	ACTOR_REGISTER_PRELOADASSETS(roomfade);
	ACTOR_REGISTER_POSTUPDATE(roomfade);
	ACTOR_REGISTER_TRANSPARENTDRAWWORLD(roomfade);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(roomfade)
{

}

ACTOR_POSTUPDATE(roomfade)
{
	struct Actor* player = FINDACTORTYPE(act_player);
	if (player == NULL)
		return;
	if (Vector3Distance(player->position, actor->position) > FADE_LIMIT)
		return;
	// If near enough, check dotproduct, if it's behind us change rooms and disable us. 
	// We should ONLY ever exist for THAT room! Don't be in a global layer!!
	if (Vector3DotProduct(Vector3RotateByQuaternion(VEC3FORWARD, actor->rotation), VEC3DIRECTION(actor->position, player->position)) > 0)
		return;
	ChangeSceneRoom(GetCurrentScene(), 1, TRUE);
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK | ACTOR_FLAG_IS_INVISIBLE;
}

ACTOR_TRANSPARENTDRAWWORLD(roomfade)
{
	if (OutOfRenderRange(actor))
		return;

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