#ifndef __LIGHT_TOOLS_HEADER__
#define __LIGHT_TOOLS_HEADER__

#include "raymath.h"
#include "game_draw.h"

#define LIGHT_NODE_SKYBOX lighting_append_light( Vector3Zero(), LIGHT_WORLD_RANGE, GetSunColor(), GetSunIntensity());

#define LIGHT_NODE_CAVE(x,y,z,r) lighting_append_light((Vector3) { x, y, z }, r, BLACK, 1.0f);

#define LIGHT_NODE_TORCH(x,y,z,r) lighting_append_light((Vector3) { x, y, z }, r, WHITE, RAND_FRANGE(0.95f, 1.0f));

#endif