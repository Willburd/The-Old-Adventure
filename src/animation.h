#ifndef ANIMATION_HEADER
#define ANIMATION_HEADER

#include "actor.h"
#include "assets.h"

struct AnimationLayer {
	unsigned int layer_index;
	ModelAnimation* current_animation;	// current animation pointer
	char* anim_name;					// string name of animation
	int is_playing;						// If animation is animating
	int single_shot;					// If animation plays once
	float blend_factor;					// Intensity of animation blended over the previous layers
	float frame_rate;					// Rate animation plays at
	float previous_frame;					
	float current_frame;
};

// Gets a stored model's animation from a known animation name
ModelAnimation* GetAnimation(Asset* asset, char* name);
// Adds an animation to the next free layer
int AddAnimLayer(struct Actor* actor, ModelAnimation* new_anim, float framerate, int single_shot);
// Gets an animation layer by it's index
struct AnimationLayer* GetAnimLayer(struct Actor* actor, unsigned int index);
// Gets a animation layer by it's animation name
struct AnimationLayer* FindAnimLayer(struct Actor* actor, char* name);
// Updates the frame data of all all layers on an actor.
void UpdateAnimLayers(struct Actor* actor);
// Blends all layers according to their blending intensity, playing state, order, etc
void ApplyAnimLayers(struct Actor* actor, Model* model);


#endif