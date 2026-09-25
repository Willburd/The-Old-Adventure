#ifndef __ACTOR_WATER_VOLUME_HEADER__
#define __ACTOR_WATER_VOLUME_HEADER__

#include "raylib.h"
#include "../actor.h"

// There should probably never be more water volumes then there are thousands of actors...
#define WATER_VOLUME_LIMIT 64

// Gets the first water volume actor found at a point, otherwise returns null.
struct Actor* PointInWaterVolume(Vector3 point);

#endif