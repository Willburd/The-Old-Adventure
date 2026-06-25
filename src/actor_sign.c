#include "core_assets.h"
#include "actor_factory.h"
#include "tools.h"
#include "actor_textbox.h"
#include "camera.h"

// private header
ACTOR_PRELOADASSETS(sign);
ACTOR_INTERACT_TEXT(sign);
ACTOR_PLAYER_INTERACT(sign);
ACTOR_DRAWWORLD(sign);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	// Start camera focus
	struct Actor* camera = FINDACTORTYPE(act_camera);
	CameraSetMode(camera, CAMERA_MODE_FOCUS_CUTSCENE_SLOW);
	SetCutsceneCameraLookPos(camera, Vector3Add(actor->position, VEC3UP));
	// Textbox display
	TEXTBOX_CREATE(actor, player, TEXT_TEST_DEBUG, TEXTBOX_DEFAULT_SPEED);
}

ACTOR_DRAWWORLD(sign)
{
	DrawSphereWires(actor->position, ACTOR_INTERACTION_RANGE, 5, 5, WHITE);
}