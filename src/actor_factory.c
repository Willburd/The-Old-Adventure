#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "tools.h"
#include "return_codes.h"
#include "actor_factory.h"
#include "game_update.h"
#include "animation.h"
#include "collision.h"
#include "actor_library.h"

// Creates an actor from json provided
struct Actor* JSON_ACTOR_FACTORY(cJSON* actor_data, struct Actor* actor_parent)
{
	if (!cJSON_IsString(cJSON_GetObjectItem(actor_data, "type")))
		return NULL;
	int actor_type = ACTOR_FROM_STRING(cJSON_GetObjectItem(actor_data, "type")->valuestring);

	Vector3 at_position = Vector3Zero();
	if (cJSON_IsArray(cJSON_GetObjectItem(actor_data, "pos")))
	{
		cJSON* pos_array = cJSON_GetObjectItem(actor_data, "pos");
		at_position = (Vector3){
			(float)cJSON_GetArrayItem(pos_array, 0)->valuedouble,
			(float)cJSON_GetArrayItem(pos_array, 1)->valuedouble,
			(float)cJSON_GetArrayItem(pos_array, 2)->valuedouble,
		};
	}

	Quaternion at_rotation = QuaternionIdentity();
	if (cJSON_IsArray(cJSON_GetObjectItem(actor_data, "rot")))
	{
		cJSON* rot_array = cJSON_GetObjectItem(actor_data, "rot");
		at_rotation = QuaternionFromEuler(
			(float)cJSON_GetArrayItem(rot_array, 0)->valuedouble * DEG2RAD,
			(float)cJSON_GetArrayItem(rot_array, 1)->valuedouble * DEG2RAD,
			(float)cJSON_GetArrayItem(rot_array, 2)->valuedouble * DEG2RAD
		);
	}

	Vector3 at_scale = Vector3One();
	if (cJSON_IsArray(cJSON_GetObjectItem(actor_data, "scl")))
	{
		cJSON* scl_array = cJSON_GetObjectItem(actor_data, "scl");
		at_scale = (Vector3){
			(float)cJSON_GetArrayItem(scl_array, 0)->valuedouble,
			(float)cJSON_GetArrayItem(scl_array, 1)->valuedouble,
			(float)cJSON_GetArrayItem(scl_array, 2)->valuedouble,
		};
	}

	Vector3 initial_velocity = Vector3Zero();
	if (cJSON_IsArray(cJSON_GetObjectItem(actor_data, "vel")))
	{
		cJSON* vel_array = cJSON_GetObjectItem(actor_data, "vel");
		initial_velocity = (Vector3){
			(float)cJSON_GetArrayItem(vel_array, 0)->valuedouble,
			(float)cJSON_GetArrayItem(vel_array, 1)->valuedouble,
			(float)cJSON_GetArrayItem(vel_array, 2)->valuedouble,
		};

	}

	return ACTOR_FACTORY(actor_data, actor_type, actor_parent, at_position, at_rotation, at_scale, initial_velocity);
}

// Creates an actor in the world.
struct Actor* ACTOR_FACTORY(cJSON* actor_data, ActorTypes actor_type, struct Actor* actor_parent, Vector3 at_position, Quaternion at_rotation, Vector3 at_scale, Vector3 initial_velocity)
{
	if (current_actor_cap >= ACTOR_LIMIT)
	{
		perror("ACTOR SPAWN:No free actor slots available!\n");
		exit(ERR_NOALLOC);
	}

	// Allocate an actor as we have a free slot
	total_actors++;
	MALLOC(struct Actor, actor, NULL);

	// Setup actor
	ACTOR_CLEAR(actor);
	actor->uuid = ++current_unique_id;
	actor->actor_type = actor_type;
	actor->parent = actor_parent;
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TEXTBOX | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_TICKDURING_CUTSCENE; // Default scene flags

	// Set position
	ACTOR_POS_SNAP(actor, at_position);
	ACTOR_ROT_SNAP(actor, at_rotation);
	ACTOR_SCALE_SNAP(actor, at_scale);
	ACTOR_VEL_RESET(actor, initial_velocity);

	// Create animation layers
	for (int i = 0; i < ANIMATION_LAYER_MAX; i++)
	{
		actor->animation_layers[i] = NULL;
	}

	// Place in update list before we configure it. This way scenes appear before their actors.
	for (int i = 0; i < ACTOR_LIMIT; i++)
	{
		if (world_actors[i] != NULL)
			continue;
		world_actors[i] = actor;
		actor->index = i;
		if (i > current_actor_cap)
			current_actor_cap = i;

	// Configure to type of actor made
	ACTOR_LIBRARY(actor, actor_type);
	if (actor_type != act_scene && ACTOR_HAS(actor, func_preloadassets)) // Scenes handle preload assets themselves at a much earlier point
		actor->func_preloadassets(actor);
	if (ACTOR_HAS(actor, func_json_init))
		actor->func_json_init(actor, actor_data);

#ifdef _DEBUG
		printf("ACTOR SPAWN: [type: %s] slot: %i [%llu]\n", actor->actor_type_name, actor->index, actor->uuid);
#endif
		return actor;
	}

	// Out of slots
	perror("ACTOR SPAWN: slot load failed!\n");
	ACTOR_DESTROY(actor);
	exit(ERR_NOALLOC);
}

// Removes an actor from the world.
void ACTOR_DESTROY(struct Actor* actor)
{
	if (!ACTOR_EXISTS(actor))
		return;
	ACTOR_DESTROY_CHILDREN(actor);
	actor->is_destroying = TRUE;
	if (ACTOR_HAS(actor, func_destroy))
		actor->func_destroy(actor);
	// Clear collisions
	CollisionCleanup(actor);
	// Clear animation layers
	for (int i = 0; i < ANIMATION_LAYER_MAX; i++)
	{
		if (actor->animation_layers[i] != NULL)
			RELEASE(actor->animation_layers[i]);
	}
}

void ACTOR_DESTROY_UUID(uint64_t uuid)
{
	for (int i = 0; i < current_actor_cap; i++)
	{
		struct Actor* check_actor = world_actors[i];
		if (!ACTOR_EXISTS(check_actor))
			continue;
		if (check_actor->uuid != uuid)
			continue;
		ACTOR_DESTROY(check_actor);
	}
}

void ACTOR_DESTROY_TYPE(ActorTypes actor_type)
{
	for (int i = 0; i < current_actor_cap; i++)
	{
		struct Actor* check_actor = world_actors[i];
		if (!ACTOR_EXISTS(check_actor))
			continue;
		if (check_actor->actor_type != actor_type)
			continue;
		ACTOR_DESTROY(check_actor);
	}
}

void ACTOR_DESTROY_ALL()
{
	for (int i = 0; i < current_actor_cap; i++)
	{
		struct Actor* check_actor = world_actors[i];
		if (!ACTOR_EXISTS(check_actor))
			continue;
		ACTOR_DESTROY(check_actor);
	}
}

void ACTOR_DESTROY_CHILDREN(struct Actor* parent)
{
	for (int i = 0; i < current_actor_cap; i++)
	{
		struct Actor* check_actor = world_actors[i];
		if (!ACTOR_EXISTS(check_actor))
			continue;
		if (ACTOR_PARENT(check_actor) != parent)
			continue;
		ACTOR_DESTROY(check_actor);
	}
}
