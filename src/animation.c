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
            layer->current_frame = fmod(layer->current_frame, anim_len);
        }
    }
}



#define ANIM_MIN_THESHOLD 0.00001f
#define CHECK_SKIP_LAYER(x) !x->is_playing || x->blend_factor <= ANIM_MIN_THESHOLD

void ApplyAnimationLayerTransformsToBone(int bone_index, Model* model, struct Actor* actor)
{
    BoneInfo* bone = &model->skeleton.bones[bone_index];

    // Reset bones to zero by applying the inverse of their bind position
    Transform* bind_transform = &model->skeleton.bindPose[bone_index];
    Matrix bind_matrix = MatrixMultiply(MatrixMultiply(
        MatrixScale(bind_transform->scale.x, bind_transform->scale.y, bind_transform->scale.z),
        QuaternionToMatrix(bind_transform->rotation)),
        MatrixTranslate(bind_transform->translation.x, bind_transform->translation.y, bind_transform->translation.z));
    model->boneMatrices[bone_index] = MatrixInvert(bind_matrix); // Apply bind pose to bone before we do any transforms

    // Summed transformation of all blended animations
    Transform blended_transform = {
        .translation = Vector3Zero(),
        .rotation = QuaternionIdentity(),
        .scale = Vector3One()
    };

    // Blend all active layers by their actual blending percents
    for (int i = 0; i <= actor->animlayer_count; i++)
    {
        if (CHECK_SKIP_LAYER(actor->animation_layers[i])) // Layer is innactive.
            continue;
        struct AnimationLayer* layer = actor->animation_layers[i];
        if (!layer->bone_filter[bone_index]) // Layer is disabled by this bone's filter.
            continue;

        // Get current bone's transform at this frame of the animation
        ModelAnimation* anim = layer->current_animation;
        int frame_index = (int)layer->current_frame;
        Transform* anim_transform = &anim->keyframePoses[frame_index][bone_index];

        // Lerp from the prior pose to the desired transform. This will act as our influence of intensity.
        blended_transform.translation = Vector3Lerp(blended_transform.translation, anim_transform->translation, layer->blend_factor);
        blended_transform.rotation = QuaternionSlerp(blended_transform.rotation, anim_transform->rotation, layer->blend_factor);
        blended_transform.scale = Vector3Lerp(blended_transform.scale, anim_transform->scale, layer->blend_factor);
    }

    /*
    // Parent bones keep the binding pose's affect
    if (bone->parent != -1)
    {
        // Remove binding pose from the final transform
        blended_transform.translation = Vector3Subtract(blended_transform.translation, bind_transform->translation);
        blended_transform.rotation = QuaternionMultiply(blended_transform.rotation, QuaternionInvert(bind_transform->rotation));
        blended_transform.scale = Vector3Multiply(blended_transform.scale, Vector3Scale(bind_transform->scale, -1));

        // Get parent's transform and merge it in
        Transform parent_transform;
        MatrixDecompose(model->boneMatrices[bone->parent], &parent_transform.translation, &parent_transform.rotation, &parent_transform.scale);

        // Apply our transform to parent's to get our real position
        blended_transform.translation = Vector3Add(blended_transform.translation, parent_transform.translation);
        blended_transform.rotation = QuaternionMultiply(blended_transform.rotation, parent_transform.rotation);
        blended_transform.scale = Vector3Multiply(blended_transform.scale, parent_transform.scale);
    }
    */

    // Apply the blending matrix to the bone's transform.
    Matrix blended_matrix = MatrixMultiply(MatrixMultiply(
        MatrixScale(blended_transform.scale.x, blended_transform.scale.y, blended_transform.scale.z),
        QuaternionToMatrix(blended_transform.rotation)),
        MatrixTranslate(blended_transform.translation.x, blended_transform.translation.y, blended_transform.translation.z));
    model->boneMatrices[bone_index] = MatrixMultiply(model->boneMatrices[bone_index], blended_matrix);
}

// Recursively process bones from the root up!
void CalculateBoneTransform(int bone_index, Model* model, struct Actor* actor, int* solved_bones[])
{
    // If we are already solved just return
    if (solved_bones[bone_index] == TRUE)
        return;
    // Time for recursion!
    BoneInfo* bone = &model->skeleton.bones[bone_index];
    if (bone->parent > -1 && !solved_bones[bone->parent]) // Solve from our parent's transform if it hasn't been solved.
        CalculateBoneTransform(bone->parent, model, actor, solved_bones);
    ApplyAnimationLayerTransformsToBone(bone_index, model, actor);
    solved_bones[bone_index] = TRUE;
}

void ApplyAnimLayers(struct Actor* actor, Model* model)
{
    if (actor->animlayer_count == -1)
        return;
    if (model->skeleton.boneCount == 0)
        return;

    // Bones need to be calculated from the bottom up.
    // So we do so recursively... Keeping track of solved bones so we can end recursive chains early.
    int solved_bones[MAX_BONES] = { FALSE };
    for (int bone_index = 0; bone_index < model->skeleton.boneCount; bone_index++)
    {
        CalculateBoneTransform(bone_index, model, actor, &solved_bones);
    }

    // Forward bones to gpu
    SkinModel(model);
}
