#include "actor.h"
#include "collision.h"
#include "globals.h"
#include <stdio.h>

typedef struct {
	struct Actor* owner;
	Mesh* mesh;
} ColliderData;

int max_collision = 0;
ColliderData world_colliders[MAX_COLLIDERS] = { NULL };

void CollisionRegister(struct Actor* owner, Mesh* collider)
{
	for (int i = 0; i < MAX_COLLIDERS; i++)
	{
		if (world_colliders[i].mesh == NULL)
		{
			world_colliders[i].owner = owner;
			world_colliders[i].mesh = collider;
			if (max_collision < i)
				max_collision = i;
			return;
		}
	}
	printf("Unable to allocate collider");
}

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
	if (max_collision > 1)
	{
		for (int i = 1; i < max_collision; i++)
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