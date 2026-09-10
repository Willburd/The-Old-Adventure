#include "timer.h"
#include "globals.h"

typedef struct {
	uint64_t time;
	uint64_t source_uuid;
	void (*func_callback)(struct Actor* actor);
	int active;
} TimerEvent;

#define MAX_TIMERS 256

int highest_timer = -1;
static TimerEvent timers[MAX_TIMERS] = { { .time = 0, .func_callback = NULL, .source_uuid = 0, .active = FALSE } };

void ProcessTimers()
{
	int new_highest_timer = -1;
	for (int i = 0; i <= highest_timer; i++)
	{
		if (!timers[i].active)
			continue;
		new_highest_timer = i;
		if (tick_counter < timers[i].time)
			continue;
		struct Actor* find_actor = FINDACTOR_BYID(timers[i].source_uuid);
		if (find_actor != NULL)
			timers[i].func_callback(find_actor);
		timers[i].active = FALSE;
	}
	highest_timer = new_highest_timer;
}

int TimerAdd(struct Actor* source, double seconds, void(*func_callback)(struct Actor* actor))
{
	if (!ACTOR_EXISTS(source))
		return;

	// Turn seconds into ticks
	uint64_t ticks = (uint64_t)(seconds * update_rate);

	// Make a timer
	for (int i = 0; i < MAX_TIMERS; i++)
	{
		if (timers[i].active)
			continue;
		timers[i].source_uuid = source->uuid;
		timers[i].time = tick_counter + ticks;
		timers[i].func_callback = func_callback;
		timers[i].active = TRUE;
		if (i > highest_timer)
			highest_timer = i;
		break;
	}
}

void TimerClear(int timer_index)
{
	timers[timer_index].active = FALSE;
}