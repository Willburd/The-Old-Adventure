#include "actor.h"
#include "tools.h"
#include "core_assets.h"
#include "light_tools.h"
#include "camera.h"
#include "rlgl.h"

// Assets
#define FIRE_MATERIAL		ASSET_MATERIALS"/Effects/fire.mat"

// private header
ACTOR_PRELOADASSETS(fire);
ACTOR_LIGHTNODES(fire);
ACTOR_TRANSPARENTDRAWWORLD(fire);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(fire)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_INTERACTIVE;
	ACTOR_REGISTER_PRELOADASSETS(fire);
	ACTOR_REGISTER_LIGHTNODES(fire);
	ACTOR_REGISTER_TRANSPARENTDRAWWORLD(fire);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(fire)
{
	LoadAsset_Material(FIRE_MATERIAL, FALSE);
}

ACTOR_LIGHTNODES(fire)
{
	LIGHT_NODE_TORCH(actor->position.x, actor->position.y, actor->position.z, 20.0f);
}

ACTOR_TRANSPARENTDRAWWORLD(fire)
{
	Transform fire_transform = {
		.translation = actor->position,
		.rotation = QuaternionLookAt( cam_main.position, actor->position, VEC3UP),
		.scale = (Vector3) { 1.0f, 2.0f, 1.0f }
	};

	////////////////////////////////////////////////
	// Fire shader uniforms
	////////////////////////////////////////////////
	Material* mat = AssetGet_Material(FIRE_MATERIAL);
	Shader set_shader = mat->shader;
	BeginShaderMode(set_shader);

	Vector4 fire_color = ColorToVector4(GOLD);
	int fire_loc = GetShaderLocation(set_shader, "FireColor");
	SetShaderValue(set_shader, fire_loc, &fire_color, SHADER_UNIFORM_VEC4);

	fire_loc = GetShaderLocation(set_shader, "Identity");
	SetShaderValue(set_shader, fire_loc, &actor->uuid, SHADER_UNIFORM_INT);

	float animation_ticker = (float)tick_counter * 0.035f;
	fire_loc = GetShaderLocation(set_shader, "Animator");
	SetShaderValue(set_shader, fire_loc, &animation_ticker, SHADER_UNIFORM_FLOAT);

	DrawMesh(
		AssetGet_Model(SPRITE_MODEL)->meshes[0],
		*AssetGet_Material(FIRE_MATERIAL),
		MATRIX_ASSEMBLE(fire_transform)
	);
	EndShaderMode();
}