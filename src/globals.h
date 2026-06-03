#ifndef GLOBALS_HEADER
#define GLOBALS_HEADER

#include <stdint.h>

#define TRUE 1
#define FALSE 0

const double update_rate;

int frame_rate;
int screenWidth;
int screenHeight;

double update_ticker;
uint64_t tick_counter;
double seconds_counter;

#endif