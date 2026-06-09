#include "actor.h"
#include "animation.h"
#include "tools.h"
#include <string.h>

static void ApplyBlendedAnim(struct Actor* actor, Model* model, struct AnimationLayer* layer, float influence, double tick_percent);

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
        layer->blend_factor = blend_factor;
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
            if (layer->current_frame > 1.0 && layer->single_shot)
            {
                layer->current_frame = 0.0;
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
    // Blend all active layers by their actual blending percents
    for (int i = 0; i <= actor->animlayer_count; i++)
    {
        if (CHECK_SKIP_LAYER(actor->animation_layers[i]))
            continue;
        struct AnimationLayer* layer = actor->animation_layers[i];
        ApplyBlendedAnim(actor, model, layer, layer->blend_factor / total_blend, tick_percent);
    }
}

// Blends animation into model with specific influence
static void ApplyBlendedAnim(struct Actor* actor, Model* model, struct AnimationLayer* layer, float influence, double tick_percent)
{
    // TEMP
    UpdateModelAnimation(*model, *layer->current_animation, layer->current_frame);
}