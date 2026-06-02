#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include <stdint.h>
#include "actor.h"
#include "raylib.h"

struct Actor;

struct Actor* ACTOR_FACTORY(actor_types actor_type, Vector3 at_position, Quaternion at_rotation, Vector3 at_scale, Vector3 initial_velocity);
inline void ACTOR_LIBRARY(struct Actor* actor, actor_types actor_type);
void ACTOR_DESTROY(struct Actor* actor);

#endif