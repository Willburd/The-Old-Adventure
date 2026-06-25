#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "actor_textbox.h"
#include "gamestate.h"
#include "hud.h"
#include "input.h"
#include <string.h>
#include "text_loading.h"

typedef struct
{
	unsigned int counter;
	unsigned int text_start_index;
	unsigned int text_end_index;
	char* current_text;
	char current_token;
} TextboxData;

// private header
ACTOR_UPDATE(textbox);
ACTOR_POSTDRAWHUD(textbox);
ACTOR_CLEANUP(textbox);
void ProcessToken(struct Actor* textbox);
void AdvanceText(struct Actor* textbox);

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
	textbox_data->text_start_index = 0;
	textbox_data->text_end_index = 0;
	textbox_data->current_text = "?";
	textbox_data->current_token = "A";
}

struct Actor* TEXTBOX_CREATE(struct Actor* owner, struct Actor* player, char* text_id)
{
	struct Actor* textbox = ACTOR_FACTORY(act_textbox, NULL, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
	TextboxData* textbox_data = (TextboxData*)textbox->data;
	textbox_data->current_text = GetText(text_id);
	AdvanceText(textbox);
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
	{
		AdvanceText(actor);
	}

	if (textbox_data->text_start_index >= strlen(textbox_data->current_text))
	{
		ACTOR_DESTROY(actor);
		return;
	}
}

ACTOR_CLEANUP(textbox)
{
	gameplay_state &= ~GAMESTATE_TEXTBOX;
	if(!(gameplay_state & GAMESTATE_CUTSCENE)) // If we are not in a cutscene
		gameplay_state |= GAMESTATE_GAMEPLAY; // Return to normal gameplay
}

ACTOR_POSTDRAWHUD(textbox)
{
	// Backing
	TextboxData* textbox_data = (TextboxData*)actor->data;
	const int text_box_width = 310;
	const int text_box_height = 80;
	int left = HUD_WIDTHHALF - (text_box_width / 2);
	int top = HUD_HEIGHT - (text_box_height + 5);
	DrawRectangle(left, top, text_box_width, text_box_height, BLACK);

	// Draw the current text segment
	unsigned int segment_length = (textbox_data->text_end_index - textbox_data->text_start_index) - 3; // 3 chars are from token + eol
	char substr[2048] = { 0 };
	memcpy(substr, textbox_data->current_text + (sizeof(char) * textbox_data->text_start_index), sizeof(char) * segment_length);
	DrawTextEx(default_font, substr, (Vector2) { left + 10, top + 5 }, TEXTBOX_DEFAULT_SIZE, 1, WHITE);
}

void ProcessToken(struct Actor* textbox)
{
	TextboxData* textbox_data = (TextboxData*)textbox->data;
}

void AdvanceText(struct Actor* textbox)
{
	TextboxData* textbox_data = (TextboxData*)textbox->data;
	textbox_data->counter = 0; // Reset timer
	textbox_data->text_start_index = textbox_data->text_end_index;

	// advance the starting point to where we last ended off
	unsigned int total_length = strlen(textbox_data->current_text);
	while (textbox_data->text_end_index < total_length)
	{
		char character = textbox_data->current_text[textbox_data->text_end_index];
		switch (character)
		{
			case '\0': // Dropout we reached the end
				return;

			case '|': // Process a token for custom text effects
				textbox_data->text_end_index++; // Skip token flag
				textbox_data->current_token = textbox_data->current_text[textbox_data->text_end_index];
				ProcessToken(textbox);
				textbox_data->text_end_index++; // skip the token
				textbox_data->text_end_index++; // skip the eol
				return;

			default: // Next character
				textbox_data->text_end_index++;
				break;
		}
	}
}