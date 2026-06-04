#ifndef ACTOR_HEADER
#define ACTOR_HEADER

#include <stdint.h>
#include "raylib.h"
#include "raymath.h"

struct Actor;
struct Actor {
	int index;
	uint64_t uuid;
	int actor_type;

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

	BoundingBox bounds;
	Mesh* collision_mesh;

	// Creation and destruction
	void (*func_init)(struct Actor* actor);
	void (*func_preloadassets)(struct Actor* actor);
	void (*func_destroy)(struct Actor* actor);
	// Update tick
	void (*func_preupdate)(struct Actor* actor);
	void (*func_update)(struct Actor* actor);
	void (*func_postupdate)(struct Actor* actor);
	// Drawing in world
	void (*func_predrawworld)(struct Actor* actor, double tick_percent);
	void (*func_drawworld)(struct Actor* actor, double tick_percent);
	void (*func_postdrawworld)(struct Actor* actor, double tick_percent);
	// Drawing on hud
	void (*func_predrawhud)(struct Actor* actor, double tick_percent);
	void (*func_drawhud)(struct Actor* actor, double tick_percent);
	void (*func_postdrawhud)(struct Actor* actor, double tick_percent);
	// Subrooms in scenes
	void (*func_activate_room)(struct Actor* actor, int room_index, int entrance);
	void (*func_deactivate_room)(struct Actor* actor, int room_index);
};
#define ACTOR_CLEAR(x) \
x->uuid = 0; \
x->index = -1; \
x->actor_type = 0; \
x->parent = NULL; \
x->func_init = NULL; \
x->func_preloadassets = NULL; \
x->func_destroy = NULL; \
x->func_preupdate = NULL; \
x->func_update = NULL; \
x->func_postupdate = NULL; \
x->func_predrawworld = NULL; \
x->func_drawworld = NULL; \
x->func_postdrawworld = NULL; \
x->func_predrawhud = NULL; \
x->func_drawhud = NULL; \
x->func_postdrawhud = NULL; \
x->func_activate_room=NULL; \
x->func_deactivate_room=NULL; \
x->data = NULL;
#define ACTOR_POS_SNAP(x, pos) x->position = pos;x->last_position = pos;
#define ACTOR_ROT_SNAP(x, rot) x->rotation = rot;x->last_rotation = rot;
#define ACTOR_SCALE_SNAP(x, scl) x->scale = scl;x->last_scale = scl;
#define ACTOR_VEL_RESET(x, vel) x->velocity = vel;x->last_velocity = vel;
#define ACTOR_EXISTS(x) (x != NULL && x->index > -1)
#define ACTOR_HAS(x, func) (x->func != NULL)

#define ACTOR_POS_DELTA(x, delta) Vector3Lerp(x->last_position, x->position, delta)
#define ACTOR_ROT_DELTA(x, delta) QuaternionLerp(x->last_rotation, x->rotation, delta)
#define ACTOR_SCALE_DELTA(x, delta) Vector3Lerp(x->last_scale, x->scale, delta)
#define ACTOR_VEL_DELTA(x, delta) Vector3Lerp(x->last_velocity, x->velocity, delta)

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
struct Actor* ACTOREXISTS(int actor_type);
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
// Sets the collision mesh and bounds from a model's mesh
void SetActorCollision(struct Actor* actor, Model* model, int mesh_index);

#endif