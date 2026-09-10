#ifndef __POST_TIMER_HEADER__
#define __POST_TIMER_HEADER__

#include "actor.h"

// Process all timers currently active.
void ProcessTimers();
// Starts a timer, returns the timer_index of the timer so it can manually be stopped.
int TimerAdd(struct Actor* source, double time, void(*func_callback)(struct Actor* actor));
// Stops a timer specified by it's timer_index.
void TimerClear(int timer_index);

#endif