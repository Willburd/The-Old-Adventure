#include "tools.h"
#include "actor.h"
#include "animation.h"
#include "input.h"

// Assets
#define MATERIAL_ANIM_TEST ASSET_MATERIALS"/Objects/skinned_example.mat"
#define MODEL_ANIM_TEST ASSET_MODELS"/Tools/robot.glb"

// private header
ACTOR_PRELOADASSETS(animationtest);
ACTOR_UPDATE(animationtest);
ACTOR_DRAWWORLD(animationtest);
ACTOR_POSTDRAWWORLD(animationtest);
ACTOR_DRAWHUD(animationtest);
ACTOR_CLEANUP(animationtest);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(animationtest)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_HAS_ANIMATIONS;
	ACTOR_REGISTER_PRELOADASSETS(animationtest);
	ACTOR_REGISTER_UPDATE(animationtest);
	ACTOR_REGISTER_DRAWWORLD(animationtest);
	//ACTOR_REGISTER_POSTDRAWWORLD(animationtest);
	//ACTOR_REGISTER_DRAWHUD(animationtest);
	ACTOR_REGISTER_CLEANUP(animationtest);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(animationtest)
{
	LoadAsset_Material(MATERIAL_ANIM_TEST, FALSE);

	Asset* model_asset = LoadAsset_Model(MODEL_ANIM_TEST, FALSE);
	struct AnimationLayer* idle_layer = AddAnimLayer(actor, GetAnimation(model_asset, "Robot_Walking"), ANIMATION_FRAMERATE, ANIM_LOOP, TRUE, 1.0f, BLENDTYPE_MIX);
	AnimLayerFilterAllBones(model_asset, idle_layer, 0.6f); // Low influence on everything except legs
	AnimLayerFilterBone(model_asset, idle_layer, "Foot.L", 1.0f);
	AnimLayerFilterBone(model_asset, idle_layer, "Foot.R", 1.0f);
	AnimLayerFilterBone(model_asset, idle_layer, "LowerLeg.L", 1.0f);
	AnimLayerFilterBone(model_asset, idle_layer, "LowerLeg.R", 1.0f);
	AnimLayerFilterBone(model_asset, idle_layer, "UpperLeg.L", 1.0f);
	AnimLayerFilterBone(model_asset, idle_layer, "UpperLeg.R", 1.0f);

	struct AnimationLayer* dance_layer = AddAnimLayer(actor, GetAnimation(model_asset, "Robot_Dance"), ANIMATION_FRAMERATE, ANIM_LOOP, TRUE, 1.0f, BLENDTYPE_MIX);
	AnimLayerFilterBone(model_asset, dance_layer, "Foot.L", 0.1f); // Low mix with anything else
	AnimLayerFilterBone(model_asset, dance_layer, "Foot.R", 0.1f);
	AnimLayerFilterBone(model_asset, dance_layer, "LowerLeg.L", 0.1f);
	AnimLayerFilterBone(model_asset, dance_layer, "LowerLeg.R", 0.1f);
	AnimLayerFilterBone(model_asset, dance_layer, "UpperLeg.L", 0.1f);
	AnimLayerFilterBone(model_asset, dance_layer, "UpperLeg.R", 0.1f);
}

ACTOR_UPDATE(animationtest)
{
	if (OutOfRenderRange(actor))
		return;
	if (CHECK_INPUTPRESSED(input_cancel))
	{
		struct AnimationLayer* layer = FindAnimLayer(actor, "Robot_Dance");
		if (layer != NULL && !layer->is_playing)
			layer->is_playing = TRUE;
	}
}

ACTOR_DRAWWORLD(animationtest)
{
	if(OutOfRenderRange(actor))
		return;
	Model* model = AssetGet_Model(MODEL_ANIM_TEST);
	Material* mat = AssetGet_Material(MATERIAL_ANIM_TEST);
	Matrix actor_matrix = GetMatrix(actor);
	ApplyAnimLayers(actor, model, mat, tick_percent);

	for (int i = 0; i <= model->meshCount; i++)
	{
		ToaDrawMesh(
			model->meshes[i],
			*mat,
			actor_matrix,
			FALSE
		);
	}
}

ACTOR_POSTDRAWWORLD(animationtest)
{
	
}

ACTOR_DRAWHUD(animationtest)
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

ACTOR_CLEANUP(animationtest)
{

}
