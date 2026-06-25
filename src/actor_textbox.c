#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "actor_textbox.h"
#include "gamestate.h"
#include "hud.h"
#include "input.h"
#include "text_loading.h"

typedef struct
{
	int counter;
	int page_index;
	char* current_text;
} TextboxData;

// private header
ACTOR_UPDATE(textbox);
ACTOR_POSTDRAWHUD(textbox);
ACTOR_CLEANUP(textbox);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(textbox)
{
	// Remove previous textboxes
	ACTOR_DESTROY_TYPE(act_textbox);

	// Lock to textbox gamestate
	gameplay_state &= ~GAMESTATE_GAMEPLAY;
	gameplay_state |= GAMESTATE_TEXTBOX;

	// Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_TEXTBOX;
	ACTOR_REGISTER_UPDATE(textbox);
	ACTOR_REGISTER_POSTDRAWHUD(textbox);
	ACTOR_REGISTER_CLEANUP(textbox);

	// Set data
	MALLOC_ACTOR_DATA(TextboxData, actor->data);
	TextboxData* textbox_data = (TextboxData*)actor->data;
	textbox_data->counter = 0;
	textbox_data->page_index = 0;
	textbox_data->current_text = "?";
}

struct Actor* TEXTBOX_CREATE(struct Actor* owner, struct Actor* player, char* text_id)
{
	struct Actor* textbox = ACTOR_FACTORY(act_textbox, NULL, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
	TextboxData* textbox_data = (TextboxData*)textbox->data;
	textbox_data->current_text = GetText(text_id);
	return textbox;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_UPDATE(textbox)
{
	TextboxData* textbox_data = (TextboxData*)actor->data;
	textbox_data->counter += 1;

	if (textbox_data->counter < 5)
		return;

	if (CHECK_INPUTPRESSED(input_interact))
		ACTOR_DESTROY(actor);
}

ACTOR_CLEANUP(textbox)
{
	gameplay_state &= ~GAMESTATE_TEXTBOX;
	if(!(gameplay_state & GAMESTATE_CUTSCENE)) // If we are not in a cutscene
		gameplay_state |= GAMESTATE_GAMEPLAY; // Return to normal gameplay
}

ACTOR_POSTDRAWHUD(textbox)
{
	TextboxData* textbox_data = (TextboxData*)actor->data;
	const int text_box_width = 310;
	const int text_box_height = 80;
	int left = HUD_WIDTHHALF - (text_box_width / 2);
	int top = HUD_HEIGHT - (text_box_height + 5);

	DrawRectangle(left, top, text_box_width, text_box_height, BLACK);
	DrawText(textbox_data->current_text, left + 5, top + 5, 12, WHITE);
}