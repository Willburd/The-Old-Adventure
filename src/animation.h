#ifndef __ANIMATION_HEADER__
#define __ANIMATION_HEADER__

// Expected animation framerate for ALL model animations
#define ANIMATION_FRAMERATE 60
#define MAX_BONES 255

#define BLENDTYPE_MIX 0
#define BLENDTYPE_REPLACE 1

#define ANIM_LOOP 0
#define ANIM_SINGLE 1

#include "actor.h"
#include "assets.h"

struct AnimationLayer {
	// Layer index that the animation is stored at
	unsigned int layer_index;
	// current animation pointer
	ModelAnimation* current_animation;	
	// If animation is animating
	int is_playing;
	// If animation plays once
	int single_shot;
	// The amount of influence a bone has on this layer
	float bone_filter[MAX_BONES];		
	// Intensity of animation blended over the previous layers
	float blend_factor;					
	// Rate animation plays at	
	double frame_rate;					
	// Current animation frame
	double current_frame;
	// Previous animation frame
	double previous_frame;
	// Blend type
	int blend_type;
};

// Gets a stored model's animation from a known animation name
ModelAnimation* GetAnimation(Asset* asset, char* name);
// Adds an animation to the next free layer
struct AnimationLayer* AddAnimLayer(struct Actor* actor, ModelAnimation* new_anim, double framerate, int single_shot, int is_playing, float blend_factor, int blend_type);
// Gets an animation layer by it's index
struct AnimationLayer* GetAnimLayer(struct Actor* actor, unsigned int index);
// Gets a animation layer by it's animation name
struct AnimationLayer* FindAnimLayer(struct Actor* actor, char* name);
// Enable or disable a bone from affecting a specific animation layer
void AnimLayerFilterBone(Asset* model_asset, struct AnimationLayer* layer, const char* bone_name, float bone_influence);
// Enables or disables all bones on a model for a specific animation layer
void AnimLayerFilterAllBones(Asset* model_asset, struct AnimationLayer* layer, float bone_influence);
// Updates the frame data of all all layers on an actor.
void UpdateAnimLayers(struct Actor* actor);
// Blends all layers according to their blending intensity, playing state, order, etc
void ApplyAnimLayers(struct Actor* actor, Model* model, double tick_percent);

// TODO - Replace this someday with gpu skinning or own implimentation
void SkinModel(Model* model);

#endif