#include "tools.h"
#include "assets.h"
#include "core_assets.h"
#include "actor.h"
#include "camera.h"
#include "gamestate.h"
#include "light_tools.h"
#include "actor_factory.h"

// Assets
#define SKYSPHERE_MODEL				ASSET_MODELS"/Tools/skysphere.glb"
#define SKYBOX_MATERIAL_CYCLE		ASSET_MATERIALS"/Skybox/sky_cycle.mat"
#define SKYBOX_MATERIAL_SUN			ASSET_MATERIALS"/Skybox/sun.mat"
#define SKYBOX_MATERIAL_MOON		ASSET_MATERIALS"/Skybox/moon.mat"

// private header
ACTOR_PRELOADASSETS(skybox);
ACTOR_UPDATE(skybox);
ACTOR_LIGHTNODES(skybox);
ACTOR_PREDRAWWORLD(skybox);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(skybox)
{
	// Remove previous skyboxes
	ACTOR_DESTROY_TYPE(act_skybox);

    // Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_TICKDURING_CUTSCENE;
	ACTOR_REGISTER_PRELOADASSETS(skybox);
	ACTOR_REGISTER_UPDATE(skybox);
	ACTOR_REGISTER_LIGHTNODES(skybox);
	ACTOR_REGISTER_PREDRAWWORLD(skybox);
	actor->scale = Vector3Scale(actor->scale, 1200.0f);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(skybox)
{
	LoadAsset_Model(SKYSPHERE_MODEL, FALSE);
	LoadAsset_Material(SKYBOX_MATERIAL_CYCLE, FALSE);
	LoadAsset_Material(SKYBOX_MATERIAL_SUN, FALSE);
	LoadAsset_Material(SKYBOX_MATERIAL_MOON, FALSE);
}

ACTOR_UPDATE(skybox)
{
	actor->rotation = QuaternionMultiply(actor->rotation, QuaternionFromEuler( 0.0f, 0.01f * DEG2RAD, 0.0f));
	fog_set(GetFogColor(), FOG_DEFAULT_POWER, GetFogDistance()); // maintain fog state with sky
}

ACTOR_LIGHTNODES(skybox)
{
	LIGHT_NODE_SKYBOX;
}

ACTOR_PREDRAWWORLD(skybox)
{
	////////////////////////////////////////////////
	// Skybox shader uniforms
	////////////////////////////////////////////////
	Material* mat = AssetGet_Material(SKYBOX_MATERIAL_CYCLE);
	Shader set_shader = mat->shader;
	BeginShaderMode(set_shader);

	// Intensities
	Vector4 sky_color = ColorToVector4(GetSkyColor());
	int sky_loc = GetShaderLocation(set_shader, "sky_color");
	SetShaderValue(set_shader, sky_loc, &sky_color, SHADER_UNIFORM_VEC4);

	Vector4 cloud_color = ColorToVector4(GetSunColor());
	sky_loc = GetShaderLocation(set_shader, "cloud_color");
	SetShaderValue(set_shader, sky_loc, &cloud_color, SHADER_UNIFORM_VEC4);

	// Snap to position of camera
	actor->position = cam_main.position;
	DrawMesh(
		AssetGet_Model(SKYSPHERE_MODEL)->meshes[0],
		*AssetGet_Material(SKYBOX_MATERIAL_CYCLE),
		GetMatrix(actor)
	);
	EndShaderMode();

	// Sun and moon orbit the world
	float orbit_angle = (daynight_cycle * 365.0f) * DEG2RAD;
	float orbit_distance = 500.0f;
	Transform sun_transform = {
		.translation = Vector3Scale(Vector3RotateByQuaternion(VEC3RIGHT, QuaternionFromEuler(0.0f, 0.0f, orbit_angle)), orbit_distance),
		.rotation = QuaternionFromVector3ToVector3(VEC3BACKWARD, VEC3DIRECTION(sun_transform.translation, Vector3Zero())),
		.scale = (Vector3){ 160, 160, 160 }
	};
	Transform moon_transform = {
		.translation = Vector3Scale(Vector3RotateByQuaternion(VEC3LEFT, QuaternionFromEuler(0.0f, 0.0f, orbit_angle)), orbit_distance),
		.rotation = QuaternionFromVector3ToVector3(VEC3BACKWARD, VEC3DIRECTION(moon_transform.translation, Vector3Zero())),
		.scale = (Vector3){ 70, 70, 70 }
	};

	DrawMesh(
		AssetGet_Model(SPRITE_MODEL)->meshes[0],
		*AssetGet_Material(SKYBOX_MATERIAL_SUN),
		MATRIX_ASSEMBLE(sun_transform)
	);
	DrawMesh(
		AssetGet_Model(SPRITE_MODEL)->meshes[0],
		*AssetGet_Material(SKYBOX_MATERIAL_MOON),
		MATRIX_ASSEMBLE(moon_transform)
	);
}
