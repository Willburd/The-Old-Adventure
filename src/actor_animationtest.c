#include "tools.h"
#include "actor.h"
#include "animation.h"
#include "input.h"

// private header
static void actor_animationtest_preload_assets(struct Actor* actor);
static void actor_animationtest_update(struct Actor* actor);
static void actor_animationtest_drawworld(struct Actor* actor, double delta_time);
static void actor_animationtest_postdrawworld(struct Actor* actor, double delta_time);
static void actor_animationtest_postdrawhud(struct Actor* actor, double delta_time);
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
	//actor->func_postdrawworld = actor_animationtest_postdrawworld;
	//actor->func_postdrawhud = actor_animationtest_postdrawhud;
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
	struct AnimationLayer* idle_layer = AddAnimLayer(actor, GetAnimation(model_asset, "Robot_Walking"), ANIMATION_FRAMERATE, FALSE, TRUE, 1.0f, BLENDTYPE_MIX);
	AnimLayerFilterAllBones(model_asset, idle_layer, 0.6); // Low influence on everything except legs
	AnimLayerFilterBone(model_asset, idle_layer, "Foot.L", 1.0);
	AnimLayerFilterBone(model_asset, idle_layer, "Foot.R", 1.0);
	AnimLayerFilterBone(model_asset, idle_layer, "LowerLeg.L", 1.0);
	AnimLayerFilterBone(model_asset, idle_layer, "LowerLeg.R", 1.0);
	AnimLayerFilterBone(model_asset, idle_layer, "UpperLeg.L", 1.0);
	AnimLayerFilterBone(model_asset, idle_layer, "UpperLeg.R", 1.0);

	struct AnimationLayer* dance_layer = AddAnimLayer(actor, GetAnimation(model_asset, "Robot_Dance"), ANIMATION_FRAMERATE, FALSE, TRUE, 1.0f, BLENDTYPE_MIX);
	AnimLayerFilterBone(model_asset, dance_layer, "Foot.L", 0.1); // Low mix with anything else
	AnimLayerFilterBone(model_asset, dance_layer, "Foot.R", 0.1);
	AnimLayerFilterBone(model_asset, dance_layer, "LowerLeg.L", 0.1);
	AnimLayerFilterBone(model_asset, dance_layer, "LowerLeg.R", 0.1);
	AnimLayerFilterBone(model_asset, dance_layer, "UpperLeg.L", 0.1);
	AnimLayerFilterBone(model_asset, dance_layer, "UpperLeg.R", 0.1);
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
	ApplyAnimLayers(actor, model, tick_percent);

	for (int i = 0; i <= model->meshCount; i++)
	{
		DrawMesh(
			model->meshes[i],
			*mat,
			actor_matrix
		);
	}
}

static void actor_animationtest_postdrawworld(struct Actor* actor, double tick_percent)
{
	
}

static void actor_animationtest_postdrawhud(struct Actor* actor, double tick_percent)
{
	Model* model = AssetGet_Model(MODEL_ANIM_TEST);
	for (int bone_index = 0; bone_index < model->skeleton.boneCount; bone_index++)
	{
		Transform anim_transform = { 0 };
		MatrixDecompose(model->boneMatrices[bone_index], &anim_transform.translation, &anim_transform.rotation, &anim_transform.scale);
		Vector3 eulars = QuaternionToEuler(anim_transform.rotation);
		DrawText(TextFormat("[%i: %s] p(%f, %f, %f) e(%f, %f, %f) s(%f, %f, %f)",
			bone_index,
			model->skeleton.bones[bone_index].name,
			anim_transform.translation.x,
			anim_transform.translation.y,
			anim_transform.translation.z,
			eulars.x,
			eulars.y,
			eulars.z,
			anim_transform.scale.x,
			anim_transform.scale.y,
			anim_transform.scale.z
		), 10, 10 + (bone_index * 11), 9, WHITE);
	}
}

static void actor_animationtest_destroy(struct Actor* actor)
{

}
