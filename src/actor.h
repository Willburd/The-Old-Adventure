#ifndef __ACTOR_HEADER__
#define __ACTOR_HEADER__

#include <stdint.h>
#include "raylib.h"
#include "raymath.h"
#include "animation.h"

#define ANIMATION_LAYER_MAX 24

// Must match gamestate flags
#define ACTOR_FLAG_TICKDURING_GAME (1 << 0) // Updates during gameplay
#define ACTOR_FLAG_TICKDURING_TRANSITION (1 << 1) // Updates during room enter/exit animations
#define ACTOR_FLAG_TICKDURING_CUTSCENE (1 << 2) // Updates cutscenes
#define ACTOR_FLAG_TICKDURING_PAUSED (1 << 3) // Updates during pause screen
// RESERVED (1 << 4)
// RESERVED (1 << 5)
// RESERVED (1 << 6)
// RESERVED (1 << 7)
// End of reserved
#define ACTOR_FLAG_HAS_ANIMATIONS (1 << 8) // Has animations
#define ACTOR_FLAG_IS_INVISIBLE (1 << 9) // disable drawing
#define ACTOR_FLAG_INTERACTIVE (1 << 10) // player can trigger interacts with

struct Actor;
struct Actor {
	int index;
	uint64_t uuid;
	int actor_type;
	int is_destroying;

	struct Actor* parent;
	void* data;

	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	Vector3 velocity;

	Vector3 last_position;
	Quaternion last_rotation;
	Vector3 last_scale;
	Vector3 last_velocity;

	unsigned int actor_flags;
	unsigned int collision_flags; // Defines in collision.h

	int animlayer_count;
	struct AnimationLayer* animation_layers[ANIMATION_LAYER_MAX];

	// Actor setup and function assignment
	void (*func_init)(struct Actor* actor);
	// Load assets when actor is created
	void (*func_preloadassets)(struct Actor* actor);
	// Actor cleanup during ACTOR_DESTROY()
	void (*func_destroy)(struct Actor* actor);
	// Actor update tick before primary update
	void (*func_preupdate)(struct Actor* actor);
	// Actor update tick
	void (*func_update)(struct Actor* actor);
	// Actor update tick after primary update
	void (*func_postupdate)(struct Actor* actor);
	// Add the lights of this object to the light queue
	void (*func_append_lights)(struct Actor* actor);
	// Actor draw world before primary draw
	void (*func_predrawworld)(struct Actor* actor, double tick_percent);
	// Actor draw world
	void (*func_drawworld)(struct Actor* actor, double tick_percent);
	// Actor draw world after primary draw
	void (*func_postdrawworld)(struct Actor* actor, double tick_percent);
	// Actor draw hud before primary draw
	void (*func_predrawhud)(struct Actor* actor, double tick_percent);
	// Actor draw hud
	void (*func_drawhud)(struct Actor* actor, double tick_percent);
	// Actor draw hud after primary draw
	void (*func_postdrawhud)(struct Actor* actor, double tick_percent);
	// Handle activating a room within a scene
	void (*func_activate_room)(struct Actor* actor, int room_index, int entrance);
	// Handle deactivating a room within a scene
	void (*func_deactivate_room)(struct Actor* actor, int room_index);
	// Handle animation end/loop actions
	void (*func_animation_ended)(struct Actor* actor, char* anim_name);
	// Handle interactions from the player
	void (*func_player_interact)(struct Actor* actor, struct Actor* player);
	// Handle interactions from other actors
	void (*func_remote_interact)(struct Actor* actor, struct Actor* other_actor);
	// Handle collisions
	void (*func_touch)(struct Actor* actor, struct Actor* other);
};
#define ACTOR_CLEAR(x) \
x->uuid = 0; x->index = -1; x->actor_type = 0; \
x->is_destroying = FALSE; \
x->parent = NULL; \
x->actor_flags = 0; \
x->actor_flags = 0; \
x->collision_flags = 0; \
x->animlayer_count = -1; \
x->func_init = NULL; x->func_preloadassets = NULL; \
x->func_destroy = NULL; \
x->func_preupdate = NULL; x->func_update = NULL; x->func_postupdate = NULL; \
x->func_append_lights = NULL; \
x->func_predrawworld = NULL; x->func_drawworld = NULL; x->func_postdrawworld = NULL; \
x->func_predrawhud = NULL; x->func_drawhud = NULL; x->func_postdrawhud = NULL; \
x->func_activate_room = NULL; x->func_deactivate_room = NULL; \
x->func_animation_ended = NULL; \
x->func_player_interact = NULL;x->func_remote_interact = NULL;x->func_touch = NULL; \
x->data = NULL;

#define ACTOR_POS_SNAP(x, pos) x->position = pos;x->last_position = pos;
#define ACTOR_ROT_SNAP(x, rot) x->rotation = rot;x->last_rotation = rot;
#define ACTOR_SCALE_SNAP(x, scl) x->scale = scl;x->last_scale = scl;
#define ACTOR_VEL_RESET(x, vel) x->velocity = vel;x->last_velocity = vel;
#define ACTOR_EXISTS(x) (x != NULL && !x->is_destroying)
#define ACTOR_HAS(x, func) (x->func != NULL)

#define ACTOR_POS_DELTA(x, delta) Vector3Lerp(x->last_position, x->position, (float)delta)
#define ACTOR_ROT_DELTA(x, delta) QuaternionLerp(x->last_rotation, x->rotation, (float)delta)
#define ACTOR_SCALE_DELTA(x, delta) Vector3Lerp(x->last_scale, x->scale, (float)delta)
#define ACTOR_VEL_DELTA(x, delta) Vector3Lerp(x->last_velocity, x->velocity, (float)delta)

#define ACTOR_PARENT(x) (x->parent)

#define ACTOR_LIMIT 8192

/// Array of all actors
struct Actor* world_actors[ACTOR_LIMIT];	
/// Current highest actor array slot in memory. Shifts up and down as actors are reallocated to blank spots to close gaps from deletion.
int current_actor_cap;						
/// Total number of actors that exist in memory
int total_actors;							
/// Incrimented for every actor made. NOT used to index world_actors.
uint64_t current_unique_id;		

// Utility
// Find an actor by its uuid
struct Actor* FINDACTOR(uint64_t id);
// Find multiple actors by their uuids
void FINDACTORGROUP(const struct Actor* found_group[], int max_count, const uint64_t find_uuids[]);
// Get first actor of a specified type
struct Actor* FINDACTORTYPE(int actor_type);
// Get all actors of a specific type
void FINDALLACTORTYPE(const struct Actor* found_group[], int max_count, int actor_type);
// Find actor nearest to a specified point, also allows filtering out a specified actor during the search
struct Actor* FINDACTORNEAREST(Vector3 at_pos, struct Actor* ignored_actor);
// Find the actor furthest from a specified point, also allows filtering out a specified actor during the search
struct Actor* FINDACTORFURTHEST(Vector3 at_pos, struct Actor* ignored_actor);
// Find the nearest actor that allows interaction, also allows filtering out a specified actor during the search
struct Actor* FINDINTERACTIONNEAREST(Vector3 at_pos, struct Actor* ignored_actor);
// Number of actors of this type
int ACTORCOUNT(int actor_type);
// Get first child of parent
struct Actor* FINDCHILD(const struct Actor* parent);
// Get all children of parent
void FINDACTORCHILDREN(const struct Actor* found_group[], int max_count, const struct Actor* parent);
// Get number of children a parent has
int CHILDCOUNT(const struct Actor* parent);
// Get parent scene
struct Actor* GETSCENE(struct Actor* actor);
// Get matrix of model
Matrix GetMatrix(struct Actor* actor);
// Get transform of model
Transform GetTransform(struct Actor* actor);

#endif