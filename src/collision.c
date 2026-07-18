#include "actor.h"
#include "collision.h"
#include "globals.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	struct Actor* owner;
	Mesh* mesh;
	unsigned int flags;
} ColliderData;

int max_collision = -1;
ColliderData world_colliders[MAX_COLLIDERS] = { NULL };

// Allocate collision to the game's collision system. DO NOT FORGET TO RESIGN IT.
void CollisionRegister(struct Actor* owner, Mesh* collider, unsigned int collision_flags)
{
	for (int i = 0; i < MAX_COLLIDERS; i++)
	{
		if (world_colliders[i].mesh != NULL)
			continue;
		// Assign a new collider to this slot
		world_colliders[i].owner = owner;
		world_colliders[i].mesh = collider;
		world_colliders[i].flags = collision_flags;
		if (max_collision < i)
			max_collision = i;
		return;
	}
	printf("Unable to allocate collider for: [%llu] %s\n", owner->uuid, owner->actor_type_name);
}

// Resigns a collision from the game's collision system.
void CollisionResign(struct Actor* owner, Mesh* collider)
{
	// Find and remove the matching entry
	int removed = FALSE;
	for (int i = 0; i < MAX_COLLIDERS; i++)
	{
		if (world_colliders[i].owner == owner && world_colliders[i].mesh == collider)
		{
			world_colliders[i].owner = NULL;
			world_colliders[i].mesh = NULL;
			removed = TRUE;
		}
	}
	if (!removed)
		return;
	// Pack the array. We can only remove one entry each time so this is sane.
	if (max_collision > 0)
	{
		for (int i = 1; i <= max_collision; i++)
		{
			// if our previous slot is open, push it back a slot.
			if (world_colliders[i - 1].mesh == NULL && world_colliders[i].mesh != NULL)
			{
				world_colliders[i - 1].owner = world_colliders[i].owner;
				world_colliders[i - 1].mesh = world_colliders[i].mesh;
				world_colliders[i].owner = NULL;
				world_colliders[i].mesh = NULL;
			}

		}
	}
	max_collision--;
}

// Cleans up all colliders tied to an actor
void CollisionCleanup(struct Actor* owner)
{
	int index_counter = 0;
	ColliderData clear_array[MAX_COLLIDERS] = { NULL };
	for (int i = 0; i <= max_collision; i++)
	{
		if (world_colliders[i].owner == owner)
			clear_array[index_counter++] = world_colliders[i];
	}
	if(!index_counter)
		return;
	for (int i = 0; i < index_counter; i++)
		CollisionResign(owner, clear_array[i].mesh);
}

// Lets keep this off the heap
static RayCollision collision_test_list[MAX_COLLIDERS];

// Get a list of collisions, tested against all active colliders. This is internal. Other functions should use it and return results. Uses an pre-allocated and cleaned array instead of a heap object due to memory footprint.
static void CollisionTest(Ray raycast, float max_dist, unsigned int mask)
{
	memset(collision_test_list, NULL, sizeof(RayCollision) * MAX_COLLIDERS);
	int detected_collisions = 0;
	for (int i = 0; i <= max_collision; i++)
	{
		if (!(world_colliders[i].flags & mask)) // Masked out
			continue;
		RayCollision hit = GetRayCollisionMesh(raycast, *world_colliders[i].mesh, GetMatrix(world_colliders[i].owner));
		if (!hit.hit)
			continue;
		if (hit.distance > max_dist)
			continue;
		collision_test_list[detected_collisions++] = hit;
	}
}

// Get the nearest point. May not return a valid collision.
RayCollision CollisionGetNearest(Ray raycast, float max_dist, unsigned int mask)
{
	CollisionTest(raycast, max_dist, mask);
	// Search for the nearest hit
	float nearest_point = INFINITY;
	RayCollision nearest = { 0 };
	for (int i = 0; i <= min(max_collision, MAX_COLLIDERS); i++)
	{
		if (!collision_test_list[i].hit)
			continue;
		if (collision_test_list[i].distance >= nearest_point)
			continue;
		nearest.distance = collision_test_list[i].distance;
		nearest.hit = collision_test_list[i].hit;
		nearest.point = collision_test_list[i].point;
		nearest.normal = collision_test_list[i].normal;
		nearest_point = nearest.distance;
	}
	return nearest;
}

// Get the furthest point. May not return a valid collision.
RayCollision CollisionGetFurthest(Ray raycast, float max_dist, unsigned int mask)
{
	CollisionTest(raycast, max_dist, mask);
	// Search for the furthest hit
	float furthest_point = 0;
	RayCollision furthest = { 0 };
	for (int i = 0; i < max_collision; i++)
	{
		if (!collision_test_list[i].hit)
			continue;
		if (collision_test_list[i].distance < furthest_point)
			continue;
		furthest.distance = collision_test_list[i].distance;
		furthest.hit = collision_test_list[i].hit;
		furthest.point = collision_test_list[i].point;
		furthest.normal = collision_test_list[i].normal;
		furthest_point = furthest.distance;
	}
	return furthest;
}