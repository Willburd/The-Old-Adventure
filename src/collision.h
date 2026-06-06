#ifndef COLLISION_HEADER
#define COLLISION_HEADER

#define MAX_COLLIDERS 256

#include "tools.h"

void CollisionRegister(struct Actor* owner, Mesh* collider);
void CollisionResign(struct Actor* owner, Mesh* collider);

#endif
