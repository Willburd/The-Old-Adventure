#include "core_assets.h"
#include "actor_factory.h"
#include "tools.h"
#include "actor_textbox.h"
#include "camera.h"

// Assets


// private header
ACTOR_PRELOADASSETS(signpost);
ACTOR_INTERACT_TEXT(signpost);
ACTOR_PLAYER_INTERACT(signpost);
ACTOR_DRAWWORLD(signpost);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(signpost)
{
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_INTERACTIVE;
	ACTOR_REGISTER_PRELOADASSETS(signpost);
	ACTOR_REGISTER_DRAWWORLD(signpost);
	ACTOR_REGISTER_PLAYER_INTERACT(signpost);
	ACTOR_REGISTER_INTERACT_TEXT(signpost);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PRELOADASSETS(signpost)
{

}

ACTOR_CAN_INTERACT(signpost)
{
	return TRUE;
}

ACTOR_INTERACT_TEXT(signpost)
{
	return TEXT_ACTIONBUTTON_READ;
}

ACTOR_PLAYER_INTERACT(signpost)
{
	// Start camera focus
	struct Actor* camera = FINDACTORTYPE(act_camera);
	CameraSetMode(camera, CAMERA_MODE_FOCUS_CUTSCENE_SLOW);
	SetCutsceneCameraLookPos(camera, Vector3Add(actor->position, VEC3UP));
	// Textbox display
	TEXTBOX_CREATE(actor, player, TEXT_TEST_DEBUG, TEXTBOX_DEFAULT_SPEED);
}

ACTOR_DRAWWORLD(signpost)
{
	DrawSphereWires(actor->position, ACTOR_INTERACTION_RANGE, 5, 5, WHITE);
}