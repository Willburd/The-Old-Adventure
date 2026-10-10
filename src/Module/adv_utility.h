#include "../tools.h"

// Creates a wobbling animation that reduces in intensity over time. Start rotation must be a static rotation, and not the current rotation or it will stack.
Quaternion WobbleRotation(Quaternion start_rot, unsigned int wobble_counter, unsigned int wobble_duration, float wobble_angle);