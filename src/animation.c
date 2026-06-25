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

struct AnimationLayer* AddAnimLayer(struct Actor* actor, ModelAnimation* new_anim, double framerate, int single_shot, int is_playing, float blend_factor, int blend_type)
{
    for (int i = 0; i < ANIMATION_LAYER_MAX; i++)
    {
        if (actor->animation_layers[i] != NULL)
            continue;
        MALLOC_SET(struct AnimationLayer, actor->animation_layers[i], NULL);
        struct AnimationLayer* layer = actor->animation_layers[i];
        layer->layer_index = i;
        layer->current_animation = new_anim;
        layer->frame_rate = framerate;
        layer->single_shot = single_shot;
        layer->current_frame = 0;
        layer->is_playing = is_playing;
        layer->blend_factor = Clamp(blend_factor, 0.0, 1.0);
        layer->blend_type = blend_type;
        for (int b = 0; b < MAX_BONES; b++)
        {
            // Automatically disable blending for bones above bone count
            layer->bone_filter[b] = (b < layer->current_animation->boneCount) ? 1.0f : 0.0f; 
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

void AnimLayerFilterBone(Asset* model_asset, struct AnimationLayer* layer, const char* bone_name, float bone_influence)
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
        layer->bone_filter[boneIndex] = bone_influence;
        return;
    }
    // If you mess up, lets help out
#ifdef _DEBUG
    printf("ANIM: Bone with filter id did not exist: %s\nBones ids are:\n", bone_name);
    for (int boneIndex = 0; boneIndex < boneCount; boneIndex++)
    {
        printf("[%i]:%s\n", boneIndex, model->skeleton.bones[boneIndex].name);
    }
#endif
}

void AnimLayerFilterAllBones(Asset* model_asset, struct AnimationLayer* layer, float bone_influence)
{
    Model* model = model_asset->mdl;
    if (model == NULL)
        return;
    int boneCount = model->skeleton.boneCount;
    if (boneCount == 0)
        return;
    for (int boneIndex = 0; boneIndex < boneCount; boneIndex++)
    {
        layer->bone_filter[boneIndex] = bone_influence;
    }
}


void UpdateAnimLayers(struct Actor* actor)
{
    if (actor->animlayer_count == -1)
        return;
    for (int i = 0; i <= actor->animlayer_count; i++)
    {
        struct AnimationLayer* layer = actor->animation_layers[i];

        if (actor->is_destroying) // Destroyed by animation
            break;
        if (layer->is_playing)
        {
            // Solve the animation frame rate to game tick rate
            layer->current_frame += 1.0 / (update_rate / layer->frame_rate); // Solve the animation framerate vs the game's tick rate, then get the per tick change in frame
            unsigned int anim_len = layer->current_animation->keyframeCount;
            // Single shot animations only play once
            if (layer->current_frame >= anim_len)
            {
                if (ACTOR_HAS(actor, func_animation_ended))
                    actor->func_animation_ended(actor, layer->current_animation->name);
                if (layer->single_shot)
                {
                    layer->current_frame = 0.0;
                    layer->is_playing = FALSE;
                    continue;
                }
            }
            // Looping animations, push back to within valid ranges. Negative values wrap around when getting frame data.
            layer->previous_frame = layer->current_frame;
            layer->current_frame = fmod(layer->current_frame, anim_len);
        }
    }
}


#define ANIM_MIN_THESHOLD 0.00001f
#define CHECK_SKIP_LAYER(x) !x->is_playing || x->blend_factor <= ANIM_MIN_THESHOLD

static inline Transform BuildDeltaTransform(int bone_index, struct AnimationLayer* layer, double tick_percent)
{
    ModelAnimation* anim = layer->current_animation;
    int last_frame = (int)layer->previous_frame % layer->current_animation->keyframeCount;
    int current_frame = (int)layer->current_frame % layer->current_animation->keyframeCount;

    return (Transform){
        .translation = Vector3Lerp(
        anim->keyframePoses[last_frame][bone_index].translation,
        anim->keyframePoses[current_frame][bone_index].translation, (float)tick_percent),
        .rotation = QuaternionSlerp(
        anim->keyframePoses[last_frame][bone_index].rotation,
        anim->keyframePoses[current_frame][bone_index].rotation, (float)tick_percent),
        .scale = Vector3Lerp(
        anim->keyframePoses[last_frame][bone_index].scale,
        anim->keyframePoses[current_frame][bone_index].scale, (float)tick_percent)
    };
}

static inline void ApplyAnimationLayerTransformsToBone(int bone_index, Model* model, struct Actor* actor, double tick_percent)
{
    // Get total blending amount
    double blend_total[MAX_BONES] = { 0 };
    for (int i = 0; i <= actor->animlayer_count; i++)
    {
        if (CHECK_SKIP_LAYER(actor->animation_layers[i]))
            continue;
        // Check if layer is disabled by this bone's filter.
        struct AnimationLayer* layer = actor->animation_layers[i];
        ModelAnimation* anim = layer->current_animation;
        if (layer->bone_filter[bone_index] < ANIM_MIN_THESHOLD)
            continue;
        blend_total[bone_index] += layer->bone_filter[bone_index] * layer->blend_factor;
    }

    // Blend all active layers by their actual blending percents
    for (int i = 0; i <= actor->animlayer_count; i++)
    {
        if (CHECK_SKIP_LAYER(actor->animation_layers[i])) 
            continue;
        // Check if layer is disabled by this bone's filter.
        struct AnimationLayer* layer = actor->animation_layers[i];
        ModelAnimation* anim = layer->current_animation;
        if (layer->bone_filter[bone_index] < ANIM_MIN_THESHOLD)
            continue;

        // Compute interpolated pose between both animations frames
        Transform frame_transform = BuildDeltaTransform(bone_index, layer, tick_percent);

        // Blend with current animation
        double blend = layer->bone_filter[bone_index] * layer->blend_factor;
        if(layer->blend_type == BLENDTYPE_MIX)
            blend /= blend_total[bone_index];
        model->currentPose[bone_index].translation = Vector3Lerp(model->currentPose[bone_index].translation, frame_transform.translation, (float)blend);
        model->currentPose[bone_index].rotation = QuaternionSlerp(model->currentPose[bone_index].rotation, frame_transform.rotation, (float)blend);
        model->currentPose[bone_index].scale = Vector3Lerp(model->currentPose[bone_index].scale, frame_transform.scale, (float)blend);
    }

    // Compute runtime bone matrix from model current pose
    Matrix bindPoseMatrix = MATRIX_ASSEMBLE((model->skeleton.bindPose[bone_index]));
    Matrix currentPoseMatrix = MATRIX_ASSEMBLE((model->currentPose[bone_index]));
    model->boneMatrices[bone_index] = MatrixMultiply(MatrixInvert(bindPoseMatrix), currentPoseMatrix);
}

void ApplyAnimLayers(struct Actor* actor, Model* model, double tick_percent)
{
    if (actor->animlayer_count == -1)
        return;
    if (model->boneMatrices == NULL)
        return;

    // Bones need to be calculated from the bottom up.
    // So we do so recursively... Keeping track of solved bones so we can end recursive chains early.
    for (int bone_index = 0; bone_index < model->skeleton.boneCount; bone_index++)
    {
        ApplyAnimationLayerTransformsToBone(bone_index, model, actor, tick_percent);
    }

    // Forward bones to gpu
    SkinModel(model);
}
