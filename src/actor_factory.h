#ifndef __ENTITY_HEADER__
#define __ENTITY_HEADER__

#include <stdint.h>
#include "cJSON.h"
#include "actor.h"
#include "actor_library.h"
#include "raylib.h"

struct Actor;

struct Actor* JSON_ACTOR_FACTORY(cJSON* file_data, struct Actor* actor_parent);
struct Actor* ACTOR_FACTORY(cJSON* file_data, ActorTypes actor_type, struct Actor* parent, Vector3 at_position, Quaternion at_rotation, Vector3 at_scale, Vector3 initial_velocity);
void ACTOR_DESTROY(struct Actor* actor);
void ACTOR_DESTROY_UUID(uint64_t uuid);
void ACTOR_DESTROY_TYPE(ActorTypes actor_type);
void ACTOR_DESTROY_ALL();
void ACTOR_DESTROY_IN_ROOM(int index);
void ACTOR_DESTROY_CHILDREN(struct Actor* parent);

void HandleActorFinalCleanup(struct Actor* goner);

#endif