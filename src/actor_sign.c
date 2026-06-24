#include "core_assets.h"
#include "actor.h"
#include "tools.h"

// private header
ACTOR_PRELOADASSETS(sign);
ACTOR_INTERACT_TEXT(sign);
ACTOR_PLAYER_INTERACT(sign);
ACTOR_DRAWWORLD(sign);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
ACTOR_INIT(sign)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_INTERACTIVE;
	ACTOR_REGISTER_PRELOADASSETS(sign);
	ACTOR_REGISTER_DRAWWORLD(sign);
	ACTOR_REGISTER_PLAYER_INTERACT(sign);
	ACTOR_REGISTER_INTERACT_TEXT(sign);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(sign)
{

}

ACTOR_CAN_INTERACT(sign)
{
	return TRUE;
}

ACTOR_INTERACT_TEXT(sign)
{
	return TEXT_ACTIONBUTTON_READ;
}

ACTOR_PLAYER_INTERACT(sign)
{
	printf("Interaction!");
}

ACTOR_DRAWWORLD(sign)
{
	DrawSphereWires(actor->position, ACTOR_INTERACTION_RANGE, 5, 5, WHITE);
}