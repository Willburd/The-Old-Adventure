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

int AddAnimLayer(struct Actor* actor, ModelAnimation* new_anim, float framerate, int single_shot);
struct AnimationLayer* GetAnimLayer(struct Actor* actor, unsigned int index);
struct AnimationLayer* FindAnimLayer(struct Actor* actor, char* name);
void UpdateAnimLayers(struct Actor* actor);

ModelAnimation* GetAnimation(Asset* asset, char* name);

#endif