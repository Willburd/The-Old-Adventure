#include "actor.h"
#include "animation.h"
#include "tools.h"
#include "raymath.h"
#include <string.h>

ModelAnimation* GetAnimation(Asset* asset, char* name)
{
    if (asset->mdl == NULL || asset->anm == NULL)
        return NULL;
    for (int i = 0; i < asset->anm_count; i++)
    {
        if (STRMATCH(asset->anm[i].name, name))
            return &asset->anm[i];
    }
    return NULL;
}

struct AnimationLayer* AddAnimLayer(struct Actor* actor, ModelAnimation* new_anim, double framerate, int single_shot, int is_playing, float blend_factor)
{
    for (int i = 0; i < ANIMATION_LAYER_MAX; i++)
    {
        if (actor->animation_layers[i] != NULL)
            continue;
        MALLOC_SET(struct AnimationLayer, actor->animation_layers[i], -1);
        struct AnimationLayer* layer = actor->animation_layers[i];
        layer->layer_index = i;
        layer->current_animation = new_anim;
        layer->frame_rate = framerate;
        layer->single_shot = single_shot;
        layer->current_frame = 0;
        layer->is_playing = is_playing;
        layer->blend_factor = Clamp(blend_factor, 0.0, 1.0);
        for (int b = 0; b < MAX_BONES; b++)
        {
            // Automatically disable blending for bones above bone count
            layer->bone_filter[b] = (b < layer->current_animation->boneCount); 
        }
        if (i > actor->animlayer_count)
            actor->animlayer_count = i;
        printf("ANIM: Layer created [%i]:%s\n", i, new_anim->name);
        return layer;
    }
    return NULL;
}

struct AnimationLayer* GetAnimLayer(struct Actor* actor, unsigned int index)
{
    return actor->animation_layers[index];
}

struct AnimationLayer* FindAnimLayer(struct Actor* actor, char* name)
{
    if (actor->animlayer_count == -1)
        return NULL;
    for (int i = 0; i <= actor->animlayer_count; i++)
    {
        if (strcmp(actor->animation_layers[i]->current_animation->name, name) == 0)
            return actor->animation_layers[i];
    }
    return NULL;
}

void AnimLayerFilterBone(Asset* model_asset, struct AnimationLayer* layer, const char* bone_name, char state)
{
    Model* model = model_asset->mdl;
    if (model == NULL)
        return;
    int boneCount = model->skeleton.boneCount;
    if (boneCount == 0)
        return;
    for (int boneIndex = 0; boneIndex < boneCount; boneIndex++)
    {
        if (!STRMATCH(model->skeleton.bones[boneIndex].name, bone_name))
            continue;
        layer->bone_filter[boneIndex] = state;
        return;
    }
    // If you mess up, lets help out
#ifdef _DEBUG
    printf("ANIM: Bone with filter id did not exist: %s\Bones ids are:\n", bone_name);
    for (int boneIndex = 0; boneIndex < boneCount; boneIndex++)
    {
        printf("[%i]:%s\n", boneIndex, model->skeleton.bones[boneIndex].name);
    }
#endif
}

void UpdateAnimLayers(struct Actor* actor)
{
    if (actor->animlayer_count == -1)
        return;
    for (int i = 0; i <= actor->animlayer_count; i++)
    {
        struct AnimationLayer* layer = actor->animation_layers[i];
        if (layer->is_playing)
        {
            // Solve the animation frame rate to game tick rate
            layer->current_frame += 1.0 / (update_rate / layer->frame_rate); // Solve the animation framerate vs the game's tick rate, then get the per tick change in frame
            unsigned int anim_len = layer->current_animation->keyframeCount;
            // Single shot animations only play once
            if (layer->current_frame >= anim_len && layer->single_shot)
            {
                layer->current_frame = 0.0;
                layer->is_playing = FALSE;
                continue;
            }
            // Looping animations, push back to within valid ranges. Negative values wrap around when getting frame data.
            layer->previous_frame = layer->current_frame;
            layer->current_frame = fmod(layer->current_frame, anim_len);
        }
    }
}


#define ANIM_MIN_THESHOLD 0.00001f
#define CHECK_SKIP_LAYER(x) !x->is_playing || x->blend_factor <= ANIM_MIN_THESHOLD

static void ApplyAnimationLayerTransformsToBone(int bone_index, Model* model, struct Actor* actor, double tick_percent)
{
    // Blend all active layers by their actual blending percents
    for (int i = 0; i <= actor->animlayer_count; i++)
    {
        if (CHECK_SKIP_LAYER(actor->animation_layers[i])) 
            continue;
        // Check if Llayer is disabled by this bone's filter.
        struct AnimationLayer* layer = actor->animation_layers[i];
        ModelAnimation* anim = layer->current_animation;
        if (!layer->bone_filter[bone_index]) 
            continue;

        // Solve the interpolated frame of this animation at the current frame tick
        int last_frame = (int)layer->previous_frame % layer->current_animation->keyframeCount;
        int current_frame = (int)layer->current_frame % layer->current_animation->keyframeCount;
        Vector3 frame_translation = Vector3Lerp(
            anim->keyframePoses[last_frame][bone_index].translation,
            anim->keyframePoses[current_frame][bone_index].translation, tick_percent);
        Quaternion frame_rotation = QuaternionSlerp(
            anim->keyframePoses[last_frame][bone_index].rotation,
            anim->keyframePoses[current_frame][bone_index].rotation, tick_percent);
        Vector3 frame_scale = Vector3Lerp(
            anim->keyframePoses[last_frame][bone_index].scale,
            anim->keyframePoses[current_frame][bone_index].scale, tick_percent);

        // Compute interpolated pose between both animations frames
        double blend = layer->blend_factor;
        model->currentPose[bone_index].translation = Vector3Lerp(model->currentPose[bone_index].translation, frame_translation, blend);
        model->currentPose[bone_index].rotation = QuaternionSlerp(model->currentPose[bone_index].rotation, frame_rotation, blend);
        model->currentPose[bone_index].scale = Vector3Lerp(model->currentPose[bone_index].scale, frame_scale, blend);
    }

    // Compute runtime bone matrix from model current pose
    Transform* bindPoseTransform = &model->skeleton.bindPose[bone_index];
    Matrix bindPoseMatrix = MatrixMultiply(
        MatrixMultiply(MatrixScale(bindPoseTransform->scale.x, bindPoseTransform->scale.y, bindPoseTransform->scale.z),
            QuaternionToMatrix(bindPoseTransform->rotation)),
        MatrixTranslate(bindPoseTransform->translation.x, bindPoseTransform->translation.y, bindPoseTransform->translation.z));

    Transform* currentPoseTransform = &model->currentPose[bone_index];
    Matrix currentPoseMatrix = MatrixMultiply(
        MatrixMultiply(MatrixScale(currentPoseTransform->scale.x, currentPoseTransform->scale.y, currentPoseTransform->scale.z),
            QuaternionToMatrix(currentPoseTransform->rotation)),
        MatrixTranslate(currentPoseTransform->translation.x, currentPoseTransform->translation.y, currentPoseTransform->translation.z));

    model->boneMatrices[bone_index] = MatrixMultiply(MatrixInvert(bindPoseMatrix), currentPoseMatrix);
}

// Recursively process bones from the root up!
static void CalculateBoneTransform(int bone_index, Model* model, struct Actor* actor, int* solved_bones[], double tick_percent)
{
    // If we are already solved just return
    if (solved_bones[bone_index] == TRUE)
        return;
    // Time for recursion!
    BoneInfo* bone = &model->skeleton.bones[bone_index];
    if (bone->parent > -1 && !solved_bones[bone->parent]) // Solve from our parent's transform if it hasn't been solved.
        CalculateBoneTransform(bone->parent, model, actor, solved_bones, tick_percent);
    ApplyAnimationLayerTransformsToBone(bone_index, model, actor, tick_percent);
    solved_bones[bone_index] = TRUE;
}

void ApplyAnimLayers(struct Actor* actor, Model* model, double tick_percent)
{
    if (actor->animlayer_count == -1)
        return;
    if (model->boneMatrices == NULL)
        return;

    // Bones need to be calculated from the bottom up.
    // So we do so recursively... Keeping track of solved bones so we can end recursive chains early.
    int solved_bones[MAX_BONES] = { FALSE };
    for (int bone_index = 0; bone_index < model->skeleton.boneCount; bone_index++)
    {
        CalculateBoneTransform(bone_index, model, actor, &solved_bones, tick_percent);
    }

    // Forward bones to gpu
    SkinModel(model);
}
