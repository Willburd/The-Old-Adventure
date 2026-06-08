#include "tools.h"
#include "actor.h"
#include "animation.h"

// private header
void actor_animationtest_preload_assets(struct Actor* actor);
void actor_animationtest_update(struct Actor* actor);
void actor_animationtest_drawworld(struct Actor* actor, double delta_time);
void actor_animationtest_destroy(struct Actor* actor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void actor_animationtest_init(struct Actor* actor)
{
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

void actor_animationtest_preload_assets(struct Actor* actor)
{
	LoadAsset_Model(MODEL_ANIM_TEST, FALSE);
}

void actor_animationtest_update(struct Actor* actor)
{

}

void actor_animationtest_drawworld(struct Actor* actor, double tick_percent)
{
	Material* mat = AssetGet_Material(MATERIAL_ANIM_TEST);

	Asset* model_asset = AssetGetPackage(MODEL_ANIM_TEST);

	ModelAnimation* anim_extend = GetAnimation(model_asset, "Extend");
	ModelAnimation* anim_sway = GetAnimation(model_asset, "Sway");

	UpdateModelAnimationEx(*model_asset->mdl, *anim_sway, rand() % anim_sway->keyframeCount, *anim_extend, rand() % anim_extend->keyframeCount, 0.5f);

	DrawMesh(
		model_asset->mdl->meshes[0],
		*mat,
		GetMatrix(actor)
	);
}

void actor_animationtest_destroy(struct Actor* actor)
{

}