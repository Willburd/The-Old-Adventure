#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "actor_fadein.h"

// private header
ACTOR_UPDATE(fadein);
ACTOR_POSTDRAWHUD(fadein);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(fadein)
{
	// Remove previous fades if somehow multiple happen
	ACTOR_DESTROY_TYPE(act_fadein);

	// Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_PAUSED;
	ACTOR_REGISTER_UPDATE(fadein);
	ACTOR_REGISTER_POSTDRAWHUD(fadein);

	// Set data
	MALLOC_ACTOR_DATA(FadeInData, actor->data);
	FadeInData* fadein_data = (FadeInData*)actor->data;

	fadein_data->blend_color = BLACK;
	fadein_data->fadeout = 255;
	fadein_data->previous_fadeout = 255;
}

struct Actor* FADEIN_CREATE(Color color)
{
	struct Actor* fadein = ACTOR_FACTORY(NULL, act_fadein, NULL, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
	FadeInData* fadein_data = (FadeInData*)fadein->data;
	fadein_data->blend_color = color;
	return fadein;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_UPDATE(fadein)
{
	// Fade into the scene
	FadeInData* fadein_data = (FadeInData*)actor->data;
	fadein_data->previous_fadeout = fadein_data->fadeout;
	fadein_data->fadeout -= 5;
	if (fadein_data->fadeout >= 0)
		return;
	// end the fade
	fadein_data->fadeout = 0;
	ACTOR_DESTROY(actor);
}

ACTOR_POSTDRAWHUD(fadein)
{
	FadeInData* fadein_data = (FadeInData*)actor->data;
	DrawRectangle(0, 0, renderWidth, renderHeight, (Color) { fadein_data->blend_color.r, fadein_data->blend_color.g, fadein_data->blend_color.b, (int)Clamp(Lerp((float)fadein_data->previous_fadeout, (float)fadein_data->fadeout, (float)tick_percent), 0, 255) });
}
