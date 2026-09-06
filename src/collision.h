#include "models.h"

#ifndef __COLLISION_HEADER__
#define __COLLISION_HEADER__

#define DEFAULT_COLLISION_MESH "Col"

#define MAX_COLLIDERS 512
#define MAX_COLLISION_RAYS 1024

#define COL_LAYER_NONE 0
#define COL_LAYER_WORLD (1 << 0)	// World geometry
#define COL_LAYER_CAMERA (1 << 1)	// Camera blocking
#define COL_LAYER_ACTOR (1 << 2)	// Actors inworld
#define COL_LAYER_PLAYER (1 << 3)	// Player hurtbox
#define COL_LAYER_ENEMY (1 << 4)	// Enemy hitboxes
#define COL_LAYER_WATER (1 << 5)	// Water geometry
#define COL_LAYER_MOVINGPLATFORM (1 << 6)	// Moving platforms
#define COL_LAYER_PROJECTILES (1 << 7)	// Projectiles
#define COL_LAYER_ALL (~0)

#define REGISTER_COLLISION_MESH(act, mdl_ast, mesh_name, layers) CollisionRegister(act, &mdl_ast->mdl->meshes[GetMeshIndex(mdl_ast->mesh_data, mesh_name)], layers);printf("COLLISION: register mesh: %s\n", mesh_name);
#define RESIGN_COLLISION_MESH(act, mdl_ast, mesh_name) CollisionResign(act, &mdl_ast->mdl->meshes[GetMeshIndex(mdl_ast->mesh_data, mesh_name)]);printf("COLLISION: resign mesh: %s\n", mesh_name);

#include "tools.h"

struct ColliderData {
	struct Actor* owner;
	Mesh* mesh;
	unsigned int flags;
};

typedef struct {
	RayCollision ray_col;
	struct ColliderData* hit_colider;
} RayHitData;

// Get the number of colliders currently registered.
int GetColliderCount();
// Gets current number of collider rays done this tick.
int GetDebugColliderRayCount();
// Reset the current debug ray counter.
void ResetDebugRayCount();
// Draws all collision rays done this update tick.
void DebugDrawCollisionRays();

// Get a collision using it's index in the collider array. Shouldn't be used for anything except iterating all colliders outside of collision.c
struct ColliderData* GetCollider(int index);
// Allocate collision to the game's collision system. DO NOT FORGET TO RESIGN IT.
void CollisionRegister(struct Actor* owner, Mesh* collider, unsigned int collision_flags);
// Resigns a collision from the game's collision system.
void CollisionResign(struct Actor* owner, Mesh* collider);
// Cleans up all colliders tied to an actor
void CollisionCleanup(struct Actor* owner);

// Get the nearest point. May not return a valid collision.
RayHitData CollisionGetNearest(Ray raycast, float max_dist, unsigned int mask);
// Get the furthest point. May not return a valid collision.
RayHitData CollisionGetFurthest(Ray raycast, float max_dist, unsigned int mask);

// Register all meshes present in a model as colliders.
void RegisterAllCollisionMeshes(struct Actor* actor, char* model_path, int collision_mask);

#endif
