#ifndef __ENTITY_HEADER__
#define __ENTITY_HEADER__

#include <stdint.h>
#include "cJSON/cJSON.h"
#include "actor.h"
#include "Module/actor_library.h"
#include "raylib.h"

struct Actor;

// Creates an actor from json provided
struct Actor* JSON_ACTOR_FACTORY(cJSON* file_data, struct Actor* actor_parent);
// Creates an actor in the world.
struct Actor* ACTOR_FACTORY(cJSON* file_data, ActorTypes actor_type, struct Actor* parent, Vector3 at_position, Quaternion at_rotation, Vector3 at_scale, Vector3 initial_velocity);
// Removes an actor from the world.
void ACTOR_DESTROY(struct Actor* actor);
// Destroy actor by it's specific uuid
void ACTOR_DESTROY_UUID(uint64_t uuid);
// Destroy actor by it's specific id_tag
void ACTOR_DESTROY_IDTAG(char* id_tag);
// Destroy all actors with a specific actor type id.
void ACTOR_DESTROY_TYPE(ActorTypes actor_type);
// Destroy all actors without distinction.
void ACTOR_DESTROY_ALL();
// Destroy all actors in the current scene's specified room.
void ACTOR_DESTROY_IN_ROOM(int index);
// Destroy all children of an actor. Optionally including itself.
void ACTOR_DESTROY_CHILDREN(struct Actor* parent);
// Wipe actor data and do final free() calls now that the actor is clean.
void HandleActorFinalCleanup(struct Actor* goner);

#endif