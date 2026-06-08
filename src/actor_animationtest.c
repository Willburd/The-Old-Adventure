#include "tools.h"
#include "assets.h"
#include "actor.h"

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
#define MODEL_ANIM_TEXT ASSET_MODELS"/Tools/animation_test.glb"

void actor_animationtest_preload_assets(struct Actor* actor)
{
	LoadAsset_Model(MODEL_ANIM_TEXT, FALSE);
}

void actor_animationtest_update(struct Actor* actor)
{

}

void actor_animationtest_drawworld(struct Actor* actor, double tick_percent)
{
	Material* mat = AssetGet_Material(MATERIAL_ANIM_TEST);

	DrawMesh(
		AssetGet_Model(MODEL_ANIM_TEXT)->meshes[0],
		*mat,
		GetMatrix(actor)
	);
}

void actor_animationtest_destroy(struct Actor* actor)
{

}