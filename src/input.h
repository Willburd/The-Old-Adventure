#ifndef INPUT_HEADER
#define INPUT_HEADER

#include "raylib.h""

typedef enum
{
	input_left,
	input_right,
	input_up,
	input_down,
	input_confirm,
	input_cancel,
	INPUT_LAST

} InputID;

#define INPUT_STATE_NOP 0
#define INPUT_STATE_PRESSED 1
#define INPUT_STATE_HELD 2
#define INPUT_STATE_RELEASED 3

int current_gamepad;
int input_state[];
Vector2 input_analog;

const int inputkeys[];
const int inputpads[];

void UpdateInputState();

#define CHECK_INPUTHELD(x) (input_state[x] == INPUT_STATE_PRESSED || input_state[x] == INPUT_STATE_HELD)
#define CHECK_INPUTPRESSED(x) (input_state[x] == INPUT_STATE_PRESSED)
#define CHECK_INPUTRELEASED(x) (input_state[x] == INPUT_STATE_RELEASED)
#define CHECK_INPUT_NOT(x) (input_state[x] == INPUT_STATE_NOP)

#endif