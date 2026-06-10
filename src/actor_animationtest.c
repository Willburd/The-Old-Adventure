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
	struct AnimationLayer* idle_layer = AddAnimLayer(actor, GetAnimation(model_asset, "Robot_Running"), ANIMATION_FRAMERATE, FALSE, TRUE, 1.0f);

	struct AnimationLayer* dance_layer = AddAnimLayer(actor, GetAnimation(model_asset, "Robot_Dance"), ANIMATION_FRAMERATE, TRUE, FALSE, 0.5f);
	AnimLayerFilterBone(model_asset, dance_layer, "Foot.L", FALSE);
	AnimLayerFilterBone(model_asset, dance_layer, "Foot.R", FALSE);
	AnimLayerFilterBone(model_asset, dance_layer, "LowerLeg.L", FALSE);
	AnimLayerFilterBone(model_asset, dance_layer, "LowerLeg.R", FALSE);
	AnimLayerFilterBone(model_asset, dance_layer, "UpperLeg.L", FALSE);
	AnimLayerFilterBone(model_asset, dance_layer, "UpperLeg.R", FALSE);
	AnimLayerFilterBone(model_asset, dance_layer, "Hips", FALSE);
	AnimLayerFilterBone(model_asset, dance_layer, "Abdomen", FALSE);
	AnimLayerFilterBone(model_asset, dance_layer, "Torso", FALSE);
	AnimLayerFilterBone(model_asset, dance_layer, "Shoulder.L", FALSE);
	AnimLayerFilterBone(model_asset, dance_layer, "Shoulder.R", FALSE);
	AnimLayerFilterBone(model_asset, dance_layer, "UpperArm.L", FALSE);
	AnimLayerFilterBone(model_asset, dance_layer, "UpperArm.R", FALSE);
}

static void actor_animationtest_update(struct Actor* actor)
{
	if (CHECK_INPUTPRESSED(input_cancel))
	{
		struct AnimationLayer* layer = FindAnimLayer(actor, "Robot_Dance");
		if (layer != NULL && !layer->is_playing)
			layer->is_playing = TRUE;
	}
}

static void actor_animationtest_drawworld(struct Actor* actor, double tick_percent)
{
	Model* model = AssetGet_Model(MODEL_ANIM_TEST);
	Material* mat = AssetGet_Material(MATERIAL_ANIM_TEST);
	Matrix actor_matrix = GetMatrix(actor);
	ApplyAnimLayers(actor, model);

	for (int i = 0; i <= model->meshCount; i++)
	{
		DrawMesh(
			model->meshes[i],
			*mat,
			actor_matrix
		);
	}
}

static void actor_animationtest_destroy(struct Actor* actor)
{

}
