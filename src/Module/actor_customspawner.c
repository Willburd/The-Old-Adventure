#include "../tools.h"
#include "../assets.h"
#include "../actor_factory.h"
#include "json_properties.h"
#include "../collision.h"

/*******************************************
		   Multi-actor spawner
Spawns actors in a spiral pattern dependant
upon a stride min/max, and the orbit gap between
spirals. The spiral will grow outwards as the
stride grows randomly between steps until the
specified number of actors is reached.
********************************************/

// private header
ACTOR_JSON_INIT(customspawner);
ACTOR_CLEANUP(customspawner);
ACTOR_PLAYER_INTERACT(customspawner);
ACTOR_REMOTE_INTERACT(customspawner);
ACTOR_CLEANUP(customspawner);
static void SpawnAction(struct Actor* actor);

typedef struct {
	char* actor_type;
	float min_stride;
	float max_stride;
	float orbit_gap_distance;
	int spawn_count;
	int ground_snap;
	cJSON* json_cache;
} SpawnerData;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(customspawner)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK | ACTOR_FLAG_IS_INVISIBLE;
	ACTOR_REGISTER_JSON_INIT(customspawner);
	ACTOR_REGISTER_PLAYER_INTERACT(customspawner);
	ACTOR_REGISTER_REMOTE_INTERACT(customspawner);
	ACTOR_REGISTER_CLEANUP(customspawner);

	// Set data
	MALLOC_ACTOR_DATA(SpawnerData, actor->data);
	SpawnerData* spawner_data = (SpawnerData*)actor->data;
	spawner_data->actor_type = NULL;
	spawner_data->min_stride = 0;
	spawner_data->max_stride = 1;
	spawner_data->orbit_gap_distance = 1;
	spawner_data->ground_snap = 1;
	spawner_data->json_cache = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_JSON_INIT(customspawner)
{
	SpawnerData* spawner_data = (SpawnerData*)actor->data;
	JSON_GET_STRING(spawner_data->actor_type, file_data, PROP_SPAWN_ACTOR_TYPE, NULL);
	JSON_GET_FLOAT(spawner_data->min_stride, file_data, PROP_SPAWN_STRIDE_MIN, 0.0f);
	JSON_GET_FLOAT(spawner_data->max_stride, file_data, PROP_SPAWN_STRIDE_MAX, 1.0f);
	JSON_GET_FLOAT(spawner_data->orbit_gap_distance, file_data, PROP_SPAWN_ORBIT_GAP_DISTANCE, 1.0f);
	JSON_GET_INT(spawner_data->spawn_count, file_data, PROP_SPAWN_ACTOR_COUNT, 0);
	spawner_data->ground_snap = JSON_GET_BOOL(file_data, PROP_SPAWN_GROUNDSNAP);
	spawner_data->json_cache = cJSON_Duplicate(file_data, TRUE);

	// Perform roomstart spawn
	SpawnAction(actor);
}

ACTOR_PLAYER_INTERACT(customspawner)
{
	SpawnAction(actor);
}

ACTOR_REMOTE_INTERACT(customspawner)
{
	SpawnAction(actor);
}

ACTOR_CLEANUP(customspawner)
{
	SpawnerData* spawner_data = (SpawnerData*)actor->data;
	RELEASE(spawner_data->actor_type);
	cJSON_Delete(spawner_data->json_cache);
	spawner_data->json_cache = NULL;
}

static void SpawnAction(struct Actor* actor)
{
	SpawnerData* spawner_data = (SpawnerData*)actor->data;
	if (spawner_data->actor_type == NULL)
		return;
	if (spawner_data->spawn_count <= 0)
		return;
	float total_stride = 0.0f;
	for (int i = 0; i < spawner_data->spawn_count; i++)
	{
		float rotation_count = total_stride / 360.0f;
		float center_out_distance = rotation_count * spawner_data->orbit_gap_distance;
		float spawn_stride = spawner_data->min_stride + fabs(fmod(GetFixedRandomFloat(actor->position, 2342 + (i * 63223)), spawner_data->max_stride - spawner_data->min_stride));
		float spawn_percent = (float)i / spawner_data->spawn_count;
		total_stride += spawn_stride * Lerp(2.0f, 1.0f - pow(spawn_percent, PI), spawn_percent);

		Vector3 final_pos = Vector3Add(actor->position, Vector3RotateByQuaternion(Vector3Scale(VEC3FORWARD, center_out_distance), QuaternionFromAxisAngle(VEC3UP, total_stride * DEG2RAD)));
		if (spawner_data->ground_snap)
		{
			float snap_dist = 10.0f;
			Ray check_ray = {
			   .position = Vector3Add(final_pos, Vector3Scale(VEC3UP,snap_dist * 0.5f)),
			   .direction = VEC3DOWN
			};
			RayHitData ray_col = CollisionGetNearest(check_ray, snap_dist, COL_LAYER_WORLD);
			if (ray_col.ray_col.hit)
				final_pos = ray_col.ray_col.point;
			else // We don't want to place anything if it's out of bounds
				continue; 
		}
		ACTOR_FACTORY(spawner_data->json_cache, ACTOR_FROM_STRING(spawner_data->actor_type), actor->parent, final_pos, actor->rotation, actor->scale, Vector3Zero(), Vector3Zero());
	}
}