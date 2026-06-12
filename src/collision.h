#ifndef __COLLISION_HEADER__
#define __COLLISION_HEADER__

#define MAX_COLLIDERS 256

#define COL_LAYER_NONE 0
#define COL_LAYER_WORLD (1 << 0)	// World geometry
#define COL_LAYER_ACTOR (1 << 1)	// Actors inworld
#define COL_LAYER_PLAYER (1 << 3)	// Player hurtbox
#define COL_LAYER_ENEMY (1 << 4)	// Enemy hitboxes
#define COL_LAYER_WATER (1 << 5)	// Water geometry
#define COL_LAYER_ALL (~0)

#include "tools.h"

void CollisionRegister(struct Actor* owner, Mesh* collider);
void CollisionResign(struct Actor* owner, Mesh* collider);

void CollisionTest(RayCollision* collisions, int max_collisions, Ray raycast, float max_dist, unsigned int mask);
RayCollision CollisionGetNearest(Ray raycast, float max_dist, unsigned int mask);
RayCollision CollisionGetFurthest(Ray raycast, float max_dist, unsigned int mask);

#endif
