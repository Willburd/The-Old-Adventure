#include "../tools.h"
#include "../assets.h"
#include "../actor_factory.h"
#include "actor_fade.h"

// private header
ACTOR_UPDATE(fadeout);
ACTOR_POSTDRAWHUD(fadeout);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(fadeout)
{
	// Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_PAUSED;
	ACTOR_REGISTER_UPDATE(fadeout);
	ACTOR_REGISTER_POSTDRAWHUD(fadeout);

	// Set data
	MALLOC_ACTOR_DATA(FadeInData, actor->data);
	FadeInData* fadeout_data = (FadeInData*)actor->data;

	fadeout_data->blend_color = BLACK;
	fadeout_data->fadeout = 0;
	fadeout_data->previous_fadeout = 0;
}

struct Actor* FADEOUT_CREATE(Color color)
{
	// Remove previous fades if somehow multiple happen
	ACTOR_DESTROY_TYPE(act_fadeout);
	struct Actor* fadeout = ACTOR_FACTORY(NULL, act_fadeout, NULL, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
	FadeInData* fadeout_data = (FadeInData*)fadeout->data;
	fadeout_data->blend_color = color;
	return fadeout;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_UPDATE(fadeout)
{
	// Fade into the scene
	FadeInData* fadeout_data = (FadeInData*)actor->data;
	fadeout_data->previous_fadeout = fadeout_data->fadeout;
	fadeout_data->fadeout += FADE_RATE;
	if (fadeout_data->fadeout < 255)
		return;
	// end the fade
	fadeout_data->fadeout = 255;
	actor->actor_flags = ACTOR_FLAG_NONE; // Stop ticking
}

ACTOR_POSTDRAWHUD(fadeout)
{
	FadeInData* fadeout_data = (FadeInData*)actor->data;
	DrawRectangle(0, 0, renderWidth, renderHeight, (Color) { fadeout_data->blend_color.r, fadeout_data->blend_color.g, fadeout_data->blend_color.b, (int)Clamp(Lerp((float)fadeout_data->previous_fadeout, (float)fadeout_data->fadeout, (float)tick_percent), 0, 255) });
}
