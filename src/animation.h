#ifndef __ANIMATION_HEADER__
#define __ANIMATION_HEADER__

// Expected animation framerate for ALL model animations
#define ANIMATION_FRAMERATE 30
#define MAX_BONES 255

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
	// If a bone is enabled for this animation (layer bone filter)
	int bone_filter[MAX_BONES];		
	// Intensity of animation blended over the previous layers
	float blend_factor;					
	// Rate animation plays at	
	double frame_rate;					
	// Current animation frame
	double current_frame;
};

// Gets a stored model's animation from a known animation name
ModelAnimation* GetAnimation(Asset* asset, char* name);
// Adds an animation to the next free layer
struct AnimationLayer* AddAnimLayer(struct Actor* actor, ModelAnimation* new_anim, double framerate, int single_shot, int is_playing, float blend_factor);
// Gets an animation layer by it's index
struct AnimationLayer* GetAnimLayer(struct Actor* actor, unsigned int index);
// Gets a animation layer by it's animation name
struct AnimationLayer* FindAnimLayer(struct Actor* actor, char* name);
// Enable or disable a bone from affecting a specific animation layer
void AnimLayerFilterBone(Asset* model_asset, struct AnimationLayer* layer, const char* bone_name, char state);
// Updates the frame data of all all layers on an actor.
void UpdateAnimLayers(struct Actor* actor);
// Blends all layers according to their blending intensity, playing state, order, etc
void ApplyAnimLayers(struct Actor* actor, Model* model);

// TODO - Replace this someday with gpu skinning or own implimentation
void SkinModel(Model* model);

#endif