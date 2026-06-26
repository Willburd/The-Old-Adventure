#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "actor_textbox.h"
#include "gamestate.h"
#include "hud.h"
#include "input.h"
#include <string.h>
#include "text_loading.h"
#include "camera.h"

typedef struct
{
	unsigned int text_start_index;
	unsigned int text_end_index;
	unsigned int token_index;
	char* current_text;
	unsigned int current_segment_index;
	char* current_segment_buffer;
	float text_progress;
	float text_speed;
} TextboxData;

// Utility
#define SEGMENT_BUFFER_SIZE 300

// private header
ACTOR_UPDATE(textbox);
ACTOR_POSTDRAWHUD(textbox);
ACTOR_CLEANUP(textbox);
static void ProgressSegment(struct Actor* textbox);
static void AdvanceText(struct Actor* textbox);

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
	textbox_data->text_start_index = 0;
	textbox_data->text_end_index = 0;
	textbox_data->current_text = "?";
	textbox_data->token_index = 0;
	textbox_data->text_progress = 0.0f;
	textbox_data->text_speed = TEXTBOX_DEFAULT_SPEED;
	textbox_data->current_segment_index = 0;
	MALLOC_SET_SIZE(char, SEGMENT_BUFFER_SIZE, textbox_data->current_segment_buffer, '\0', NULL);
}

struct Actor* TEXTBOX_CREATE(struct Actor* owner, struct Actor* player, char* text_id, float text_speed)
{
	struct Actor* textbox = ACTOR_FACTORY(act_textbox, NULL, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
	TextboxData* textbox_data = (TextboxData*)textbox->data;
	textbox_data->current_text = GetText(text_id);
	textbox_data->text_speed = text_speed;
	ProgressSegment(textbox);
	return textbox;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_UPDATE(textbox)
{
	TextboxData* textbox_data = (TextboxData*)actor->data;
	textbox_data->text_progress += textbox_data->text_speed;
	if (textbox_data->text_progress < 1)
		return;

	if (CHECK_INPUTPRESSED(input_interact))
	{
		unsigned int segment_length = textbox_data->current_segment_index;
		if (textbox_data->text_progress < segment_length)
		{
			// Text skip
			textbox_data->text_progress = 10000.0f;
		}
		else
		{
			// Next
			ProgressSegment(actor);
		}
	}
}

ACTOR_CLEANUP(textbox)
{
	// Clear the malloced buffer
	TextboxData* textbox_data = (TextboxData*)actor->data;
	RELEASE(textbox_data->current_segment_buffer);
	// Return gameplay
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
	unsigned int segment_length = textbox_data->current_segment_index;
	if (textbox_data->text_progress > segment_length)
		textbox_data->text_progress = segment_length;
	unsigned int draw_length = (unsigned int)textbox_data->text_progress;

	char substr[SEGMENT_BUFFER_SIZE] = { 0 }; // Only needs to contain the current segment, lets not blow out the size of the heap
	memcpy(substr, textbox_data->current_segment_buffer, sizeof(char) * draw_length);
	DrawTextEx(default_font, substr, (Vector2) { left + 10, top + 5 }, TEXTBOX_DEFAULT_SIZE, 1, WHITE);
}

// Processes the current token stored to perform an action. If the textbox isn't closed it will set the next text segment bounds.
static void ProgressSegment(struct Actor* textbox)
{
	TextboxData* textbox_data = (TextboxData*)textbox->data;
	char token = 'A';
	// starting a textbox will always use default unless first entry is a token
	if(textbox_data->token_index > 0) 
		token = textbox_data->current_text[textbox_data->token_index];

	printf("Activate token: %c\n", token);
	switch (token)
	{
		default:
		case 'A': // Advance
		{
			textbox_data->text_progress = 0.0f;
			textbox_data->current_segment_index = 0;
			AdvanceText(textbox);
		}
		break;

		case 'E': // End
		{
			struct Actor* camera = FINDACTORTYPE(act_camera);
			CameraSetMode(camera, CAMERA_MODE_FOLLOW);
			ACTOR_DESTROY(textbox);
		}
		break;

		case 'C': // Start cutscene
		{
			gameplay_state |= GAMESTATE_CUTSCENE; // engage cutscene mode
			ACTOR_DESTROY(textbox);
		}
		break;
	}
}

// Returns true if the token ends the current text segment. Handles mid-text token behaviors like speeding up or slowing down text speed.
static int ScanToken(struct Actor* textbox, unsigned int index)
{
	TextboxData* textbox_data = (TextboxData*)textbox->data;
	textbox_data->token_index = index;
	char token = textbox_data->current_text[index];
	printf("Processed token: %c\n", token);

	switch (token)
	{
	case 'A': // Advance
	case 'E': // End
	case 'C': // Start cutscene
		textbox_data->current_segment_buffer[textbox_data->current_segment_index++] = '\0'; // ensure current buffer is ended
		return TRUE;

	default:
		return FALSE;
	case 'S': // Slow text speed
		textbox_data->text_speed = TEXTBOX_DEFAULT_SPEED / 4.0f;
		return FALSE;
	case 'N': // Normal text speed
		textbox_data->text_speed = TEXTBOX_DEFAULT_SPEED;
		return FALSE;
	case 'F': // Fast text speed
		textbox_data->text_speed = 10.0f;
		return FALSE;
	}
}

void AdvanceText(struct Actor* textbox)
{
	TextboxData* textbox_data = (TextboxData*)textbox->data;
	textbox_data->text_progress = 0.0f;
	textbox_data->text_start_index = textbox_data->text_end_index;

	// advance the starting point to where we last ended off
	unsigned int total_length = strlen(textbox_data->current_text);
	while (textbox_data->text_end_index < total_length)
	{
		char character = textbox_data->current_text[textbox_data->text_end_index];
		switch (character)
		{
			case '\0': // Dropout we reached the end
				textbox_data->current_segment_buffer[textbox_data->current_segment_index++] = '\0'; // ensure buffer is ended
				return;

			case '[': // Process a token for custom text effects
				textbox_data->text_end_index++; // Skip token indicator
				int token_ends_segment = ScanToken(textbox, textbox_data->text_end_index++);
				// skip new line characters
				if(textbox_data->current_text[textbox_data->text_end_index] == '\n')
					textbox_data->text_end_index++; 
				// Not every token ends the segment
				if(token_ends_segment)
					return;
				break;

			default: // Next character
				textbox_data->current_segment_buffer[textbox_data->current_segment_index++] = character;
				textbox_data->text_end_index++;
				break;
		}
	}
}