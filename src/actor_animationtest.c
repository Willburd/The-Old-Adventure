#include "tools.h"
#include "actor.h"
#include "animation.h"

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
#define MODEL_ANIM_TEST ASSET_MODELS"/Tools/animation_test.glb"

static void actor_animationtest_preload_assets(struct Actor* actor)
{
	Asset* model_asset = LoadAsset_Model(MODEL_ANIM_TEST, FALSE);
	ModelAnimation* anim_extend = GetAnimation(model_asset, "Extend");
	ModelAnimation* anim_sway = GetAnimation(model_asset, "Sway");

	AddAnimLayer(actor, anim_extend, 30, FALSE, TRUE, 1.0f);
	AddAnimLayer(actor, anim_sway, 30, FALSE, TRUE, 1.0f);
}

static void actor_animationtest_update(struct Actor* actor)
{

}

static void actor_animationtest_drawworld(struct Actor* actor, double tick_percent)
{
	Model* model = AssetGet_Model(MODEL_ANIM_TEST);
	ApplyAnimLayers(actor, model, tick_percent);
	DrawMesh(
		model->meshes[0],
		*AssetGet_Material(MATERIAL_ANIM_TEST),
		GetMatrix(actor)
	);
}

static void actor_animationtest_destroy(struct Actor* actor)
{

}
