#include <stdlib.h>
#include "input.h"
#include "globals.h"
#include "raymath.h"

const int inputkeys[INPUT_LAST] = {
	KEY_LEFT,						// Move vec
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,
	KEY_KP_4,						// Camera vec
	KEY_KP_6,
	KEY_KP_8,
	KEY_KP_2,
	KEY_Z,							// confirm
	KEY_X,							// cancel
};

const int inputpads[INPUT_LAST] = { 
	0,								// Move vec
	0,
	0,
	0,
	0,								// Camera vec
	0,
	0,
	0,
	GAMEPAD_BUTTON_RIGHT_FACE_DOWN,	// confirm
	GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,// cancel
};

int current_gamepad = 0;
int input_state[INPUT_LAST] = { 0 };
Vector2 input_analog = { 0,0 };
Vector2 input_camera = { 0,0 };

float cam_speed_keyboard = 5.0f;
float cam_speed_mouse = 0.01f;
float cam_speed_gamepad = 0.10f;

int camerax_inverted = FALSE;
int cameray_inverted = TRUE; // Pref set

Vector2 mouse_pos = { 0,0 };
Vector2 mouse_last_pos = { 0,0 };
Vector2 mouse_velocity = { 0,0 };

#define CHECK_KEY(x) inputkeys[x] && IsKeyDown(inputkeys[x])
#define CHECK_PAD(x) inputpads[x] && IsGamepadButtonDown(current_gamepad, inputpads[inp])
#define DEADZONE_P(x) (x < deadzone)
#define DEADZONE_N(x) (x < -deadzone)

void UpdateInputState()
{
	const float deadzone = 0.1f;
	// Update mouse state
	mouse_last_pos.x = mouse_pos.x;
	mouse_last_pos.y = mouse_pos.y;
	mouse_pos = GetMousePosition();
	mouse_velocity = (Vector2){ mouse_last_pos.x - mouse_pos.x, mouse_last_pos.y - mouse_pos.y };

	/////////////////////////////////////////
	// Update analog from keyboard
	/////////////////////////////////////////
	input_analog.x = 0;
	input_analog.y = 0;
	// Keyboard inputs
	if (CHECK_KEY(input_left))
		input_analog.x -= 1;
	if (CHECK_KEY(input_right))
		input_analog.x += 1;
	if (CHECK_KEY(input_up))
		input_analog.y -= 1;
	if (CHECK_KEY(input_down))
		input_analog.y += 1;
	// Try gamepad
	if (input_analog.x == 0 && input_analog.y == 0)
	{
		input_analog.x = GetGamepadAxisMovement(current_gamepad, GAMEPAD_AXIS_LEFT_X);
		if (fabs(input_analog.x) < deadzone)
			input_analog.x = 0.0f;
		input_analog.y = GetGamepadAxisMovement(current_gamepad, GAMEPAD_AXIS_LEFT_Y);
		if (fabs(input_analog.y) < deadzone)
			input_analog.y = 0.0f;
	}

	/////////////////////////////////////////
	// Update camera from mouse
	/////////////////////////////////////////
	input_camera.x = 0;
	input_camera.y = 0;
	// Keyboard inputs
	if (CHECK_KEY(input_camleft))
		input_camera.x -= cam_speed_keyboard;
	if (CHECK_KEY(input_camright))
		input_camera.x += cam_speed_keyboard;
	if (CHECK_KEY(input_camup))
		input_camera.y -= cam_speed_keyboard;
	if (CHECK_KEY(input_camdown))
		input_camera.y += cam_speed_keyboard;
	// Try gamepad camera
	if (input_camera.x == 0 && input_camera.y == 0)
	{
		input_camera.x = GetGamepadAxisMovement(current_gamepad, GAMEPAD_AXIS_RIGHT_X);
		if (fabs(input_camera.x) < deadzone)
			input_camera.x = 0.0f;
		input_camera.y = GetGamepadAxisMovement(current_gamepad, GAMEPAD_AXIS_RIGHT_Y);
		if (fabs(input_camera.y) < deadzone)
			input_camera.y = 0.0f;
		// camspeed AFTER for deadzone sanity
		input_camera.x *= cam_speed_gamepad;
		input_camera.y *= cam_speed_gamepad;
	}
	// Mouse camera
	if (input_camera.x == 0 && input_camera.y == 0 && Vector2Length(mouse_velocity) >= deadzone)
	{
		input_camera.x = mouse_velocity.x;
		input_camera.y = mouse_velocity.y;
		if (fabs(input_camera.x) < deadzone)
			input_camera.x = 0.0f;
		if (fabs(input_camera.y) < deadzone)
			input_camera.y = 0.0f;
		// camspeed AFTER for deadzone sanity
		input_camera.x *= cam_speed_mouse;
		input_camera.y *= cam_speed_mouse;
	}
	// Camera inversions
	if(!camerax_inverted)
		input_camera.x *= -1.0f;
	if (!cameray_inverted)
		input_camera.y *= -1.0f;


	/////////////////////////////////////////
	// Update presses
	/////////////////////////////////////////
	for (int inp = 0; inp < INPUT_LAST; inp++)
	{
		int is_pressed = FALSE;
		if (inp <= input_down)
		{
			switch (inp)
			{
			// Analog input check
			case input_left:
				if (!DEADZONE_N(input_analog.x))
					is_pressed = TRUE;
				break;
			case input_right:
				if (!DEADZONE_P(input_analog.x))
					is_pressed = TRUE;
				break;
			case input_up:
				if (!DEADZONE_N(input_analog.y))
					is_pressed = TRUE;
				break;
			case input_down:
				if (!DEADZONE_P(input_analog.y))
					is_pressed = TRUE;
				break;
			// Camera input check
			case input_camleft:
				if (!DEADZONE_N(input_camera.x))
					is_pressed = TRUE;
				break;
			case input_camright:
				if (!DEADZONE_P(input_camera.x))
					is_pressed = TRUE;
				break;
			case input_camup:
				if (!DEADZONE_N(input_camera.y))
					is_pressed = TRUE;
				break;
			case input_camdown:
				if (!DEADZONE_P(input_camera.y))
					is_pressed = TRUE;
				break;
			}
		}
		else
		{
			// Digital check
			if (CHECK_KEY(inp))
				is_pressed = TRUE;
			if (CHECK_PAD(inp))
				is_pressed = TRUE;
		}

		// Update button state
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