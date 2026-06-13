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

#endif