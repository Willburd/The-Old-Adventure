#include "../actor_factory.h"
#include "../tools.h"
#include "../collision.h"
#include "../game_draw.h"
#include "core_assets.h"
#include "actor_water_volume.h"

// Assets
#define WATER_MODEL ASSET_MODELS"/Objects/water_volume.glb"

// private header
int total_water_volumes = -1;
static struct Actor* all_water_volumes[WATER_VOLUME_LIMIT];
ACTOR_PRELOADASSETS(water_volume);
ACTOR_DRAWWORLD(water_volume);
ACTOR_CLEANUP(water_volume);
static void AddWaterVolume(struct Actor* actor);
static void RemoveWaterVolume(struct Actor* actor);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(water_volume)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
	ACTOR_REGISTER_PRELOADASSETS(water_volume);
	ACTOR_REGISTER_DRAWWORLD(water_volume);
	ACTOR_REGISTER_CLEANUP(water_volume);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(water_volume)
{
	// Load model
	LoadAsset_Model(WATER_MODEL, FALSE);

	AddWaterVolume(actor);
}

ACTOR_DRAWWORLD(water_volume)
{
	Asset* model_asset = AssetGetPackage(WATER_MODEL);
	STANDARD_SHADER_DRAW(actor, model_asset, ASSET_MATERIALS"/Engine/example.mat", "Surface");
}

ACTOR_CLEANUP(water_volume)
{
	RemoveWaterVolume(actor);
}

static void AddWaterVolume(struct Actor* actor)
{
	for (int i = 0; i < WATER_VOLUME_LIMIT; i++)
	{
		if (all_water_volumes[i] != NULL)
			continue;
		all_water_volumes[i] = actor;
		if (i > total_water_volumes)
			total_water_volumes = i;
		break;
	}
}

static void RemoveWaterVolume(struct Actor* actor)
{
	int new_highest_water = -1;
	for (int i = 0; i <= total_water_volumes; i++)
	{
		if (all_water_volumes[i] == NULL) // Nothing here, move on.
			continue;
		if(all_water_volumes[i] == actor) // It's the actor being removed, this is now null.
		{
			all_water_volumes[i] = NULL;
			continue;
		}
		new_highest_water = i;
	}
	total_water_volumes = new_highest_water;
}

struct Actor* PointInWaterVolume(Vector3 point)
{
	if(total_water_volumes < 0)
		return NULL;
	for (int i = 0; i <= total_water_volumes; i++)
	{
		if (all_water_volumes[i] == NULL) // Nothing here, move on.
			continue;

		struct Actor* water = all_water_volumes[i];
		if (point.y < water->position.y)
			return NULL;
		if (point.y >= (water->position.y + water->scale.y))
			return NULL;
		Rectangle xz_rect = (Rectangle){ -water->position.x, -water->position.z, water->scale.x, water->scale.z };
		if (!CheckCollisionPointRec((Vector2) { point.x, point.z }, xz_rect))
			return NULL;
		return water;
	}
	return NULL;
}