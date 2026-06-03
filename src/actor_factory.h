#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include <stdint.h>
#include "actor.h"
#include "actor_library.h"
#include "raylib.h"

struct Actor;

struct Actor* ACTOR_FACTORY(ActorTypes actor_type, struct Actor* parent, Vector3 at_position, Quaternion at_rotation, Vector3 at_scale, Vector3 initial_velocity);
void ACTOR_DESTROY(struct Actor* actor);
void ACTOR_DESTROY_UUID(uint64_t uuid);
void ACTOR_DESTROY_ALL();
void ACTOR_DESTROY_CHILDREN(struct Actor* parent);

#endif