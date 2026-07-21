#include "actor.h"
#include "camera.h"
#include "tools.h"
#include "scene_entry.h"

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

void FINDACTORGROUP( const struct Actor* found_group[], int max_count, const uint64_t find_uuids[])
{
	if (find_uuids == NULL || found_group == NULL)
		return;
	// Fill the results with known values
	for (int i = 0; i <= max_count; i++)
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
		for (int t = 0; t <= max_count; t++)
		{
			uint64_t check_id = find_uuids[t];
			if (find_actor->uuid != check_id)
				continue;
			found_group[collected_index++] = find_actor;
			if (collected_index < max_count)
				continue;
			return;
		}
	}
}

struct Actor* FINDACTORTYPE(int actor_type)
{
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* find_actor = world_actors[i];
		if (find_actor == NULL)
			continue;
		if (find_actor->actor_type == actor_type)
			return find_actor;
	}
	return NULL;
}

void FINDALLACTORTYPE(const struct Actor* found_group[], int max_count, int actor_type)
{
	// Fill the results with known values
	for (int i = 0; i < max_count; i++)
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
		if (find_actor->actor_type != actor_type)
			continue;
		found_group[collected_index++] = find_actor;
		if (collected_index < max_count)
			continue;
		return;
	}
}

struct Actor* FINDACTORNEAREST(Vector3 at_pos, struct Actor* ignored_actor)
{
	float distance = FP_INFINITE;
	struct Actor* nearest_actor = NULL;
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* find_actor = world_actors[i];
		if (find_actor == NULL || find_actor == ignored_actor)
			continue;
		float found_dist = Vector3Distance(find_actor->position, at_pos);
		if(found_dist < distance)
		{
			distance = found_dist;
			nearest_actor = find_actor;
		}
	}
	return nearest_actor;
}

struct Actor* FINDACTORFURTHEST(Vector3 at_pos, struct Actor* ignored_actor)
{
	float distance = -1;
	struct Actor* furthest_actor = NULL;
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* find_actor = world_actors[i];
		if (find_actor == NULL || find_actor == ignored_actor)
			continue;
		float found_dist = Vector3Distance(find_actor->position, at_pos);
		if (found_dist > distance)
		{
			distance = found_dist;
			furthest_actor = find_actor;
		}
	}
	return furthest_actor;
}

struct Actor* FINDINTERACTIONNEAREST(Vector3 at_pos, struct Actor* ignored_actor)
{
	float distance = FP_INFINITE;
	struct Actor* nearest_actor = NULL;
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* find_actor = world_actors[i];
		if (find_actor == NULL || find_actor == ignored_actor)
			continue;
		if (!(find_actor->actor_flags & ACTOR_FLAG_INTERACTIVE))
			continue;
		float found_dist = Vector3Distance(find_actor->position, at_pos);
		if (found_dist < distance)
		{
			distance = found_dist;
			nearest_actor = find_actor;
		}
	}
	return nearest_actor;
}

int ACTORCOUNT(int actor_type)
{
	int count = 0;
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* find_actor = world_actors[i];
		if (find_actor == NULL)
			continue;
		if (find_actor->actor_type == actor_type)
			count++;
	}
	return count;
}

struct Actor* FINDCHILD(const struct Actor* parent)
{
	if (parent == NULL)
		return NULL;
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* find_actor = world_actors[i];
		if (find_actor == NULL)
			continue;
		if (ACTOR_PARENT(find_actor) == parent)
			return find_actor;
	}
	return NULL;
}

void FINDACTORCHILDREN(const struct Actor* found_group[], int max_count, const struct Actor* parent)
{
	if (parent == NULL || found_group == NULL)
		return;
	// Fill the results with known values
	for (int i = 0; i < max_count; i++)
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
		if (ACTOR_PARENT(find_actor) != parent)
			continue;
		found_group[collected_index++] = find_actor;
		if (collected_index < max_count)
			continue;
		return;
	}
}

int CHILDCOUNT(const struct Actor* parent)
{
	if (parent == NULL)
		return 0;
	int count = 0;
	for (int i = 0; i <= current_actor_cap; i++)
	{
		struct Actor* find_actor = world_actors[i];
		if (find_actor == NULL)
			continue;
		if (ACTOR_PARENT(find_actor) == parent)
			count++;
	}
	return count;
}

struct Actor* GETSCENE(struct Actor* actor)
{
	if (actor == NULL)
		return NULL;
	while (ACTOR_PARENT(actor) != NULL)
	{
		actor = ACTOR_PARENT(actor);
	}
	return actor;
}

Matrix GetMatrix(struct Actor* actor)
{
	return MatrixCompose(actor->position, actor->rotation, actor->scale);
}

Transform GetTransform(struct Actor* actor)
{
	return (Transform) { actor->position, actor->rotation, actor->scale };
}

int OutOfRenderRange(struct Actor* actor)
{
	return (Vector3Distance(cam_main.position, actor->position) > actor->draw_range);
}

void SceneFlagTrigger(struct Actor* actor)
{
	struct Actor* scene = GETSCENE(actor);
	if (!scene)
		return;
	SceneData* data = scene->data;
	if (actor->flags_permanent)
		data->perm_flags |= actor->triggers_flags;
	else
		data->temp_flags |= actor->triggers_flags;
}

void SceneFlagClear(struct Actor* actor)
{
	struct Actor* scene = GETSCENE(actor);
	if (!scene)
		return;
	SceneData* data = scene->data;
	if (actor->flags_permanent)
		data->perm_flags &= ~actor->triggers_flags;
	else
		data->temp_flags &= ~actor->triggers_flags;
}

void SceneFlagToggle(struct Actor* actor)
{
	struct Actor* scene = GETSCENE(actor);
	if (!scene)
		return;
	SceneData* data = scene->data;
	if (actor->flags_permanent)
		data->perm_flags ^= actor->triggers_flags;
	else
		data->temp_flags ^= actor->triggers_flags;
}