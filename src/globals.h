#ifndef __GLOBALS_HEADER__
#define __GLOBALS_HEADER__

#include <stdint.h>

#define TRUE 1
#define FALSE 0

#define GRAVITY -0.026f;

const double update_rate;

int frame_rate;
int screenWidth;
int screenHeight;

// Internal render size, width is solved by screen aspect ratio
int renderHeight;
int renderWidth;

double update_ticker;
uint64_t tick_counter;
double seconds_counter;

// Must match actor flags for gamestate
#define GAMESTATE_GAMEPLAY	(1 << 0)
#define GAMESTATE_TEXTBOX   (1 << 1)
#define GAMESTATE_TRANSITION (1 << 2)
#define GAMESTATE_CUTSCENE	(1 << 3)
#define GAMESTATE_PAUSED	(1 << 4)
// RESERVED (1 << 5)
// RESERVED (1 << 6)
// RESERVED (1 << 7)
#define GAMESTATE_FILTER_MODES 0xFF 

int gameplay_state;

#endif