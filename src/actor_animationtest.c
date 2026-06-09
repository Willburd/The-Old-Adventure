#include "tools.h"
#include "actor.h"
#include "animation.h"
#include "input.h"

// private header
static void actor_animationtest_preload_assets(struct Actor* actor);
static void actor_animationtest_update(struct Actor* actor);
static void actor_animationtest_drawworld(struct Actor* actor, double delta_time);
static void actor_animationtest_destroy(struct Actor* actor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void actor_animationtest_init(struct Actor* actor)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_HAS_ANIMATIONS;
	actor->func_preloadassets = actor_animationtest_preload_assets;
	actor->func_update = actor_animationtest_update;
	actor->func_drawworld = actor_animationtest_drawworld;
	actor->func_destroy = actor_animationtest_destroy;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MATERIAL_ANIM_TEST ASSET_MATERIALS"/Objects/example.mat"
#define MODEL_ANIM_TEST ASSET_MODELS"/Tools/robot.glb"

static void actor_animationtest_preload_assets(struct Actor* actor)
{
	Asset* model_asset = LoadAsset_Model(MODEL_ANIM_TEST, FALSE);
	ModelAnimation* anim_idle = GetAnimation(model_asset, "Robot_Idle");
	ModelAnimation* anim_dance = GetAnimation(model_asset, "Robot_Dance");

	AddAnimLayer(actor, anim_idle, ANIMATION_FRAMERATE, FALSE, TRUE, 1.0f);
	AddAnimLayer(actor, anim_dance, ANIMATION_FRAMERATE, TRUE, FALSE, 1.0f);
}

static void actor_animationtest_update(struct Actor* actor)
{
	if (CHECK_INPUTPRESSED(input_confirm))
	{
		struct AnimationLayer* layer = FindAnimLayer(actor, "Robot_Dance");
		if (!layer->is_playing)
			layer->is_playing = TRUE;
	}
}

static void actor_animationtest_drawworld(struct Actor* actor, double tick_percent)
{
	Model* model = AssetGet_Model(MODEL_ANIM_TEST);
	Material* mat = AssetGet_Material(MATERIAL_ANIM_TEST);
	Matrix solved_mat = GetMatrix(actor);

	ApplyAnimLayers(actor, model, tick_percent);
	for (int i = 0; i <= model->meshCount; i++)
	{
		DrawMesh(
			model->meshes[i],
			*mat,
			solved_mat
		);
	}
}

static void actor_animationtest_destroy(struct Actor* actor)
{

}
