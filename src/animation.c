#include "actor.h"
#include "animation.h"
#include "tools.h"
#include <string.h>

int AddAnimLayer(struct Actor* actor, ModelAnimation* new_anim, float framerate, int single_shot)
{
    for (int i = 0; i < ANIMATION_LAYER_MAX; i++)
    {
        if (actor->animation_layers[i] != NULL)
            continue;
        MALLOC_SET(struct AnimationLayer, actor->animation_layers[i], -1);
        struct AnimationLayer* layer = actor->animation_layers[i];
        layer->layer_index = i;
        layer->current_animation = new_anim;
        layer->anim_name = new_anim->name;
        layer->frame_rate = framerate;
        layer->single_shot = single_shot;
        layer->previous_frame = 0;
        layer->current_frame = 0;
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
    for (int i = 0; i < ANIMATION_LAYER_MAX; i++)
    {
        if (actor->animation_layers[i] == NULL)
            continue;
        if (strcmp(actor->animation_layers[i]->anim_name, name) == 0)
            return actor->animation_layers[i];
    }
    return NULL;
}

void UpdateAnimLayers(struct Actor* actor)
{
    for (int i = 0; i < ANIMATION_LAYER_MAX; i++)
    {
        if (actor->animation_layers[i] == NULL)
            continue;
        struct AnimationLayer* layer = actor->animation_layers[i];
        if (layer->is_playing)
        {
            layer->previous_frame = layer->current_frame;
            layer->current_frame += layer->frame_rate;
            unsigned int anim_len = layer->current_animation->keyframeCount;
            // Single shot animations only play once
            if (layer->current_frame > 1.0f && layer->single_shot)
            {
                layer->current_frame = 0.0f;
                layer->is_playing = FALSE;
                continue;
            }
            // Looping animations, push back to within valid ranges. Negative values wrap around when getting frame data.
            while (layer->current_frame > anim_len)
            {
                layer->current_frame -= anim_len;
                layer->previous_frame -= anim_len;
            }
        }
    }
}

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