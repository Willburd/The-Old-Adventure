#include "tools.h"
#include "assets.h"
#include "actor.h"
#include "camera.h"
#include "core_assets.h"
#include "gamestate.h"
#include "light_tools.h"
#include "actor_factory.h"

// private header
static void actor_skybox_update(struct Actor* actor);
static void actor_skybox_lights(struct Actor* scene);
static void actor_skybox_predrawworld(struct Actor* actor, double tick_percent);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void actor_skybox_init(struct Actor* actor)
{
	// Remove previous skyboxes
	ACTOR_DESTROY_TYPE(act_skybox);

    // Configure actor
	actor->func_update = actor_skybox_update;
	actor->func_append_lights = actor_skybox_lights;
    actor->func_predrawworld = actor_skybox_predrawworld;
	actor->scale = Vector3Scale(actor->scale, 1200.0f);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void actor_skybox_update(struct Actor* actor)
{
	actor->rotation = QuaternionMultiply(actor->rotation, QuaternionFromEuler( 0.0f, 0.01f * DEG2RAD, 0.0f));
	fog_set(GetFogColor(), FOG_DEFAULT_POWER, GetFogDistance()); // maintain fog state with sky
}

static void actor_skybox_lights(struct Actor* scene)
{
	LIGHT_NODE_SKYBOX;
}

static void actor_skybox_predrawworld(struct Actor* actor, double tick_percent)
{
	////////////////////////////////////////////////
	// Skybox shader uniforms
	////////////////////////////////////////////////
	Material* mat = AssetGet_Material(SKYBOX_MATERIAL_CYCLE);
	Shader set_shader = mat->shader;

	////////////////////////////////////////////////
	// Set shader mode.
	////////////////////////////////////////////////
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
}
