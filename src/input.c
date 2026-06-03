#include "input.h"
#include "globals.h"

const int inputkeys[INPUT_LAST] = {
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,
	KEY_Z,							// confirm
	KEY_X,							// cancel
};

const int inputpads[INPUT_LAST] = { 
	0,
	0,
	0,
	0,
	GAMEPAD_BUTTON_LEFT_FACE_DOWN,	// confirm
	GAMEPAD_BUTTON_LEFT_FACE_RIGHT,	// cancel
};

int current_gamepad = 0;
int input_state[INPUT_LAST] = { 0 };
Vector2 input_analog = { 0,0 };

void UpdateInputState()
{
	const float deadzone = 0.1;

	// Update analog from keyboard
	input_analog.x = 0;
	input_analog.y = 0;
	if (inputkeys[input_left] && IsKeyDown(inputkeys[input_left]))
		input_analog.x -= 1;
	if (inputkeys[input_right] && IsKeyDown(inputkeys[input_right]))
		input_analog.x += 1;
	if (inputkeys[input_up] && IsKeyDown(inputkeys[input_up]))
		input_analog.y -= 1;
	if (inputkeys[input_down] && IsKeyDown(inputkeys[input_down]))
		input_analog.y += 1;
	// Try gamepad
	if (input_analog.x == 0 && input_analog.y == 0)
	{
		input_analog.x = GetGamepadAxisMovement(current_gamepad, GAMEPAD_AXIS_LEFT_X);
		if (abs(input_analog.x) < deadzone)
			input_analog.x = 0;
		input_analog.y = GetGamepadAxisMovement(current_gamepad, GAMEPAD_AXIS_LEFT_Y);
		if (abs(input_analog.y) < deadzone)
			input_analog.y = 0;
	}

	// Update presses
	for (int inp = 0; inp < INPUT_LAST; inp++)
	{

		int is_pressed = FALSE;
		if (inp <= input_down)
		{
			// Analog input check
			switch (inp)
			{
			case input_left:
				if (input_analog.x < -deadzone)
					is_pressed = TRUE;
				break;
			case input_right:
				if (input_analog.x > deadzone)
					is_pressed = TRUE;
				break;
			case input_up:
				if (input_analog.y < -deadzone)
					is_pressed = TRUE;
				break;
			case input_down:
				if (input_analog.y > deadzone)
					is_pressed = TRUE;
				break;
			}
		}
		else
		{
			// Digital check
			if (inputkeys[inp] && IsKeyDown(inputkeys[inp]))
				is_pressed = TRUE;
			if (inputpads[inp] && IsGamepadButtonDown(current_gamepad, inputpads[inp]))
				is_pressed = TRUE;
		}

		if (is_pressed)
		{
			switch (input_state[inp])
			{
			case INPUT_STATE_NOP:
				input_state[inp] = INPUT_STATE_PRESSED;
				break;
			case INPUT_STATE_HELD:
			case INPUT_STATE_PRESSED:
				input_state[inp] = INPUT_STATE_HELD;
				break;
			case INPUT_STATE_RELEASED:
				input_state[inp] = INPUT_STATE_NOP;
				break;
			}
		}
		else
		{
			switch (input_state[inp])
			{
			case INPUT_STATE_NOP:
			case INPUT_STATE_RELEASED:
				input_state[inp] = INPUT_STATE_NOP;
				break;
			case INPUT_STATE_HELD:
			case INPUT_STATE_PRESSED:
				input_state[inp] = INPUT_STATE_RELEASED;
				break;
			}
		}
	}
}