#include "../tools.h"
#include "../assets.h"
#include "../actor_factory.h"
#include "actor_fade.h"

/*******************************************
				Fade In
Black hud layer effect that fades to transparent
then destroys itself.
********************************************/

// private header
ACTOR_UPDATE(fadein);
ACTOR_POSTDRAWHUD(fadein);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(fadein)
{
	// Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_PAUSED;
	actor->draw_range = INFINITY;
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
	// Remove previous fades if somehow multiple happen
	ACTOR_DESTROY_TYPE(act_fadein);
	struct Actor* fadein = FAST_ACTOR(act_fadein, NULL);
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
	fadein_data->fadeout -= FADE_RATE;
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
