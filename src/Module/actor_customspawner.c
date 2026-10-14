#include "../tools.h"
#include "../assets.h"
#include "../actor_factory.h"
#include "json_properties.h"
#include "../collision.h"

// private header
ACTOR_JSON_INIT(customspawner);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(customspawner)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK | ACTOR_FLAG_IS_INVISIBLE;
	ACTOR_REGISTER_JSON_INIT(customspawner);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_JSON_INIT(customspawner)
{
	char* JSON_GET_STRING(actor_type, file_data, PROP_SPAWN_ACTOR_TYPE, NULL);
	float JSON_GET_FLOAT(min_stride, file_data, PROP_SPAWN_STRIDE_MIN, 0.0f);
	float JSON_GET_FLOAT(max_stride, file_data, PROP_SPAWN_STRIDE_MAX, 1.0f);
	float JSON_GET_FLOAT(orbit_gap_distance, file_data, PROP_SPAWN_ORBIT_GAP_DISTANCE, 1.0f);
	float JSON_GET_INT(spawn_count, file_data, PROP_SPAWN_ACTOR_COUNT, 0);
	int ground_snap = JSON_GET_BOOL(file_data, PROP_SPAWN_GROUNDSNAP);

	if (actor_type == NULL)
		return;
	if (spawn_count <= 0)
		return;
	float total_stride = 0;
	for (int i = 0; i < spawn_count; i++)
	{
		struct Actor* spawned = ACTOR_FACTORY(file_data, ACTOR_FROM_STRING(actor_type), actor->parent, actor->position, actor->rotation, actor->scale, Vector3Zero(), Vector3Zero());
		float spawn_stride = min_stride + fabs(fmod(GetFixedRandomFloat(actor->position, 2342 + (i * 63223)), max_stride - min_stride));
		total_stride += spawn_stride;
		
		float rotation_count = total_stride / 360.0f;
		float center_out_distance = rotation_count * orbit_gap_distance;
		Vector3 final_pos = Vector3Add(actor->position, Vector3RotateByQuaternion(Vector3Scale(VEC3FORWARD, center_out_distance), QuaternionFromAxisAngle(VEC3UP, total_stride * DEG2RAD)));
		if (ground_snap)
		{
			float snap_dist = 10.0f;
			Ray check_ray = {
			   .position = Vector3Add(final_pos, Vector3Scale(VEC3UP,snap_dist * 0.5f)),
			   .direction = VEC3DOWN
			};
			RayHitData ray_col = CollisionGetNearest(check_ray, snap_dist, COL_LAYER_WORLD);
			if (ray_col.ray_col.hit)
				final_pos = ray_col.ray_col.point;
		}
		ACTOR_POS_SNAP(spawned, final_pos);
		// Reapply random variation
		if (ACTOR_HAS(spawned, func_json_init))
			spawned->func_json_init(spawned, file_data);
	}
	
	// We're finished, mark us for destruction
	RELEASE(actor_type);
	ACTOR_DESTROY(actor);
}