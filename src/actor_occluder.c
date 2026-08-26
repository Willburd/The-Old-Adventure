#include "actor_factory.h"
#include "tools.h"
#include "game_draw.h"
#include "core_assets.h"
#include "actor_scene.h"
#include "json_properties.h"
#include <math.h>

// Utility
#define FADE_LIMIT actor->scale.z

// Assets
#define BLEND_MATERIAL_MAIN ASSET_MATERIALS"/Engine/color_blend.mat"

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
	struct Actor* player = FINDACTOR_BYTYPE(act_player);
	if (player == NULL)
		return;
	float dist = Vector3FlatDistance(player->position, actor->position);
	if (dist > FADE_LIMIT)
		return;
	if (Vector3DotProduct(Vector3RotateByQuaternion(VEC3FORWARD, actor->rotation), VEC3DIRECTION(actor->position, player->position)) < 0)
	{
		actor->blend_color.w = 0.0f;
		return;
	}
	actor->blend_color.w = Clamp(dist / FADE_LIMIT, 0.0f, 1.0f);
}

ACTOR_TRANSPARENTDRAWWORLD(occluder)
{
	Material* transition_mat = AssetGet_Material(BLEND_MATERIAL_MAIN);
	shader_update_defaultuniforms(transition_mat->shader, actor);

	int blend_loc = GetShaderLocation(transition_mat->shader, "blend_color");
	SetShaderValue(transition_mat->shader, blend_loc, &actor->blend_color, SHADER_UNIFORM_VEC4);

	DrawMesh(
		AssetGet_Model(SPRITE_MODEL)->meshes[0],
		*transition_mat,
		GetMatrix(actor)
	);

	if (!draw_debug_info)
		return;
	DrawSphereWires(actor->position, FADE_LIMIT, 10, 10, BLUE);
}