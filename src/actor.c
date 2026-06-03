#include "actor.h"

struct Actor* FINDACTOR(uint64_t find_uuid)
{
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* find_actor = world_actors[i];
		if (find_actor == NULL)
			continue;
		if (find_actor->uuid == find_uuid)
			return find_actor;
	}
	return NULL;
}

void FINDACTORGROUP( const struct Actor* found_group[], const uint64_t find_uuids[])
{
	if (find_uuids == NULL || found_group == NULL)
		return;
	// Fill the results with known values
	int search_arr_len = sizeof(find_uuids) / sizeof(uint64_t);
	for (int i = 0; i <= search_arr_len; i++)
	{
		found_group[i] = NULL; 
	}
	// Search all entities till we find our targets
	int collected_index = 0;
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* find_actor = world_actors[i];
		if (find_actor == NULL)
			continue;
		for (int t = 0; t <= search_arr_len; t++)
		{
			uint64_t check_id = find_uuids[t];
			if (find_actor->uuid != check_id)
				continue;
			found_group[collected_index++] = find_actor;
			if (collected_index >= search_arr_len)
				return;
		}
	}
}