#include "actor.h"
#include "animation.h"
#include "tools.h"
#include <string.h>

ModelAnimation* GetAnimation(Asset* asset, char* name)
{
    if (asset->mdl == NULL || asset->anm == NULL)
        return NULL;
    for (int i = 0; i < asset->anm_count; i++)
    {
        if (strcmp(asset->anm[i].name, name) == 0)
            return &asset->anm[i];
    }
    return NULL;
}

int AddAnimLayer(struct Actor* actor, ModelAnimation* new_anim, double framerate, int single_shot, int is_playing, float blend_factor)
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
        layer->previous_frame = 0;
        layer->current_frame = 0;
        layer->is_playing = is_playing;
        layer->blend_factor = Clamp(blend_factor, 0.0, 1.0);
        if (i > actor->animlayer_count)
            actor->animlayer_count = i;
        printf("ANIM: Layer created [%i]:%s\n", i, new_anim->name);
        return i;
    }
    return -1;
}

struct AnimationLayer* GetAnimLayer(struct Actor* actor, unsigned int index)
{
    return actor->animation_layers[index];
}

struct AnimationLayer* FindAnimLayer(struct Actor* actor, char* name)
{
    if (actor->animlayer_count == -1)
        return;
    for (int i = 0; i <= actor->animlayer_count; i++)
    {
        if (strcmp(actor->animation_layers[i]->current_animation->name, name) == 0)
            return actor->animation_layers[i];
    }
    return NULL;
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
            layer->previous_frame = layer->current_frame;
            layer->current_frame += 1.0 / (update_rate / layer->frame_rate); // Solve the animation framerate vs the game's tick rate, then get the per tick change in frame
            unsigned int anim_len = layer->current_animation->keyframeCount;
            // Single shot animations only play once
            if (layer->current_frame >= 1.0 && layer->single_shot)
            {
                layer->current_frame = 0.0;
                layer->is_playing = FALSE;
                continue;
            }
            // Looping animations, push back to within valid ranges. Negative values wrap around when getting frame data.
            while (layer->current_frame >= anim_len)
            {
                layer->current_frame -= anim_len;
                layer->previous_frame -= anim_len;
            }
        }
    }
}

#define ANIM_MIN_THESHOLD 0.00001f
#define CHECK_SKIP_LAYER(x) !x->is_playing || x->blend_factor <= ANIM_MIN_THESHOLD

void ApplyAnimLayers(struct Actor* actor, Model* model, double tick_percent)
{
    if (actor->animlayer_count == -1)
        return;
    // Get the total blending factor of all active layers
    float total_blend = 0.0f;
    for (int i = 0; i <= actor->animlayer_count; i++)
    {
        if(CHECK_SKIP_LAYER(actor->animation_layers[i]))
            continue;
        total_blend += actor->animation_layers[i]->blend_factor;
    }
    // Somehow nothing was active...
    if (total_blend < ANIM_MIN_THESHOLD)
        return;
    int boneCount = model->skeleton.boneCount;
    if (boneCount == 0)
        return;

    // For each bone in the skeleton, apply the blended animation
    for (int boneIndex = 0; boneIndex < boneCount; boneIndex++)
    {
        // Get bind pose for the bone.
        Transform* bind_transform = &model->skeleton.bindPose[boneIndex];
        Matrix bind_matrix = MatrixMultiply(MatrixMultiply(
            MatrixScale(bind_transform->scale.x, bind_transform->scale.y, bind_transform->scale.z),
            QuaternionToMatrix(bind_transform->rotation)),
            MatrixTranslate(bind_transform->translation.x, bind_transform->translation.y, bind_transform->translation.z));
        model->boneMatrices[boneIndex] = MatrixInvert(bind_matrix); // Apply bind pose to bone before we do any transforms

        // Blend all active layers by their actual blending percents
        for (int i = 0; i <= actor->animlayer_count; i++)
        {
            if (CHECK_SKIP_LAYER(actor->animation_layers[i]))
                continue;
            // Get layer's data
            struct AnimationLayer* layer = actor->animation_layers[i];
            ModelAnimation* anim = layer->current_animation;
            double layer_blend = layer->blend_factor / total_blend;

            // Get current bone's transform at this frame of the animation
            int frame_index = (int)layer->current_frame;
            Transform* anim_transform = &anim->keyframePoses[frame_index][boneIndex];

            // Lerp from the bind pose to the desired transform. This will act as our influence of intensity.
            Transform blended = { 0 };
            blended.translation = Vector3Lerp(bind_transform->translation, anim_transform->translation, layer_blend);
            blended.rotation = QuaternionSlerp(bind_transform->rotation, anim_transform->rotation, layer_blend);
            blended.scale = Vector3Lerp(bind_transform->scale, anim_transform->scale, layer_blend);

            // Build and apply the influence scaled matrix to the bone's transform. Happens for each layer cumulatively.
            Matrix blended_matrix = MatrixMultiply(MatrixMultiply(
                MatrixScale(blended.scale.x, blended.scale.y, blended.scale.z),
                QuaternionToMatrix(blended.rotation)),
                MatrixTranslate(blended.translation.x, blended.translation.y, blended.translation.z));
            model->boneMatrices[boneIndex] = MatrixMultiply(model->boneMatrices[boneIndex], blended_matrix);
        }
        
        // Forward bones to gpu
        SkinModel(model);
    }
}