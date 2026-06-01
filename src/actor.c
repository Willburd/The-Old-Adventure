#include "actor.h"

struct Actor* FINDACTOR(uint64_t id)
{
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* find_actor = world_actors[i];
		if (find_actor == NULL)
			continue;
		if (find_actor->uuid == id)
			return find_actor;
	}
	return NULL;
}