#ifndef __LIGHT_TOOLS_HEADER__
#define __LIGHT_TOOLS_HEADER__

#include "raymath.h"
#include "game_draw.h"

#define LIGHT_NODE_SKYBOX AppendLight( Vector3Zero(), LIGHT_WORLD_RANGE, GetSunColor(), GetSunIntensity());

#define LIGHT_NODE_CAVE(x,y,z,r) AppendLight((Vector3) { x, y, z }, r, BLACK, 1.0f);

#define LIGHT_NODE_TORCH(x,y,z,r,c) AppendLight((Vector3) { x, y, z }, r, c, RAND_FRANGE(0.95f, 1.0f));

#endif