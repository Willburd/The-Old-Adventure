#include "../actor_factory.h"
#include "../tools.h"
#include "../collision.h"
#include "../game_draw.h"
#include "json_properties.h"

#define WOBBLE_DURATION 90

// Assets
#define BUSH_MODEL ASSET_MODELS"/Trees/bush_A.glb"
#define BRANCH_MATERIAL_PREFIX ASSET_MATERIALS"/Trees/branches_"

// private header
ACTOR_JSON_INIT(bush);
ACTOR_PRELOADASSETS(bush);
ACTOR_UPDATE(bush);
ACTOR_DRAWWORLD(bush);

typedef struct
{
	unsigned int wobble_counter;
} BushData;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(bush)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME|ACTOR_FLAG_TICKDURING_CUTSCENE|ACTOR_FLAG_TICKDURING_TRANSITION;
	actor->draw_range = DEFAULT_MAX_RENDER_RANGE * 0.30f;
	ACTOR_REGISTER_JSON_INIT(bush);
	ACTOR_REGISTER_PRELOADASSETS(bush);
	ACTOR_REGISTER_UPDATE(bush);
	ACTOR_REGISTER_DRAWWORLD(bush);

	// Set data
	MALLOC_ACTOR_DATA(BushData, actor->data);
	BushData* bush_data = actor->data;
	bush_data->wobble_counter = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_JSON_INIT(bush)
{
	if (JSON_GET_BOOL(file_data, PROP_RANDOMVARIATION))
	{
		actor->rotation = QuaternionMultiply(actor->rotation, 
			QuaternionFromEuler(
				(-1.0f + fmod(GetFixedRandomFloat(actor->position, 712.3f), 2.0f)) * DEG2RAD,
				fmod(GetFixedRandomFloat(actor->position, 2321.4f), 360.0f) * DEG2RAD,
				(-1.0f + fmod(GetFixedRandomFloat(actor->position, 217.3f), 2.0f)) * DEG2RAD));
		actor->scale = Vector3Scale(actor->scale, 0.95f + fmod(GetFixedRandomFloat(actor->position, 821.2f), 0.1f));
	}
}

ACTOR_PRELOADASSETS(bush)
{
	// Load model
	Asset* model_asset = LoadAsset_Model(BUSH_MODEL, FALSE);

	// Load Materials
	LoadAsset_Material(ALTSKIN(BRANCH_MATERIAL_PREFIX, actor->skin_select, ".mat"), FALSE);
}

ACTOR_UPDATE(bush)
{
	BushData* bush_data = actor->data;
	if (OutOfRenderRange(actor))
	{
		bush_data->wobble_counter = 0;
		return;
	}
	if (bush_data->wobble_counter > 0)
	{
		// Wobble animation
		bush_data->wobble_counter += 1;
		float intensity = 1.0f - ((float)bush_data->wobble_counter / (float)WOBBLE_DURATION);
		intensity *= 2.5f; // angle of wobble
		actor->rotation = QuaternionMultiply(actor->home_rotation, QuaternionFromAxisAngle(VEC3FORWARD, sinf((float)bush_data->wobble_counter / 6.0f) * intensity * DEG2RAD));
		actor->rotation = QuaternionMultiply(actor->rotation, QuaternionFromAxisAngle(VEC3RIGHT, cosf((float)bush_data->wobble_counter / 7.0f) * intensity * DEG2RAD));

		if (bush_data->wobble_counter < WOBBLE_DURATION)
			return;
		bush_data->wobble_counter = WOBBLE_DURATION;

		// Try to reset our wobble
		struct Actor* player = FINDACTOR_BYTYPE(act_player);
		if (!ACTOR_EXISTS(player))
			return;
		if (Vector3Distance(actor->position, player->position) <= 1.1f) // Don't reset if player is still standing in us
			return;
		bush_data->wobble_counter = 0;
		actor->rotation = actor->home_rotation;
	}

	// Check if the player has crossed the bush
	struct Actor* player = FINDACTOR_BYTYPE(act_player);
	if (!ACTOR_EXISTS(player))
		return;
	if (Vector3Distance(actor->position, player->position) > 1.0f)
		return;
	bush_data->wobble_counter = 1;
}

ACTOR_DRAWWORLD(bush)
{
	if (OutOfRenderRange(actor))
		return;
	Asset* model_asset = AssetGetPackage(BUSH_MODEL);
	STANDARD_SHADER_DRAW(actor, model_asset, ALTSKIN(BRANCH_MATERIAL_PREFIX, actor->skin_select, ".mat"), "Bush-Branches", tick_percent);
}