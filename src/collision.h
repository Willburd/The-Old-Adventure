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

#define REGISTER_COLLISION_MESH(act, mdl_ast, mesh_name, layers) CollisionRegister(act, &mdl_ast->mdl->meshes[GetMeshIndex(mdl_ast->mesh_data, mesh_name)], layers);

#include "tools.h"

struct ColliderData {
	struct Actor* owner;
	Mesh* mesh;
	unsigned int flags;
} ;

int max_collision;

int debug_current_rays;
Vector3 debug_ray_starts[];
Vector3 debug_ray_ends[];
int debug_ray_hits[];

struct ColliderData* GetCollider(int index);
void CollisionRegister(struct Actor* owner, Mesh* collider, unsigned int collision_flags);
void CollisionResign(struct Actor* owner, Mesh* collider);
void CollisionCleanup(struct Actor* owner);

RayCollision CollisionGetNearest(Ray raycast, float max_dist, unsigned int mask);
RayCollision CollisionGetFurthest(Ray raycast, float max_dist, unsigned int mask);

#endif
