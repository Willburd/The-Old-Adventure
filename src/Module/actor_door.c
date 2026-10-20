#include "../tools.h"
#include "../assets.h"
#include "../actor_factory.h"
#include "../scene_entry.h"
#include "../game_draw.h"
#include "../collision.h"

/*******************************************
				Door
Actor used for cosmetically entering and exiting
rooms. Player actor "interlocks" with another door
upon entering the newly loaded room, if no room is
loaded. The player will simply finish the animation.
This does not load scenes, the player must enter
a scene trigger placed behind the door during the
opening animation. This is so the doors can be used
without triggering scene load/unloads.

-TODO
********************************************/

// Assets
#define DOOR_MODEL ASSET_MODELS"/Objects/door.glb"
static const char* loaded_materials[] = {
	ASSET_MATERIALS"/Objects/door_wood_A.mat"
};

// private header
ACTOR_JSON_INIT(door);
ACTOR_PRELOADASSETS(door);
ACTOR_DRAWWORLD(door);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(door)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
	ACTOR_REGISTER_PRELOADASSETS(door);
	ACTOR_REGISTER_JSON_INIT(door);
	ACTOR_REGISTER_DRAWWORLD(door);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_JSON_INIT(door)
{
	if (file_data == NULL)
		return;

}

ACTOR_PRELOADASSETS(door)
{
	// Load model
	LoadAsset_Model(DOOR_MODEL, FALSE);

	// Load Materials
	LoadMaterialArray(loaded_materials, ARRAY_LENGTH(loaded_materials));

	// Set collision data
	RegisterAllCollisionMeshes(actor, DOOR_MODEL, COL_LAYER_WORLD | COL_LAYER_CAMERA);
}

ACTOR_DRAWWORLD(door)
{
	DrawAllModelMeshes(actor, DOOR_MODEL, loaded_materials, tick_percent);
}