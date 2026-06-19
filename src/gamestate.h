#ifndef __GAMESTATE_HEADER__
#define __GAMESTATE_HEADER__

#include "tools.h"
#include "raylib.h"

#define DEFAULT_DAYNIGHT_SPEED 0.000085f;

// Must match actor flags for gamestate
#define GAMESTATE_GAMEPLAY	(1 << 0)
#define GAMESTATE_TRANSITION (1 << 1)
#define GAMESTATE_CUTSCENE	(1 << 2)
#define GAMESTATE_PAUSED	(1 << 3)
// RESERVED (1 << 4)
// RESERVED (1 << 5)
// RESERVED (1 << 6)
// RESERVED (1 << 7)
#define GAMESTATE_FILTER_MODES 0xFF 

int gameplay_state;

#define TIME_ONEHOUR 0.041666f
#define TIME_TWOHOURS 0.083333f
#define TIME_THREEHOURS 0.125
#define TIME_SIXHOURS 0.25f)
#define TIME_HALFDAY 0.5f
#define TIME_ONEDAY 1.0f
#define TIME_DAWN 0.0f
#define TIME_NOON 0.25f
#define TIME_DUSK 0.5f
#define TIME_MIDNIGHT 0.75f
float daynight_cycle;
float daynight_speed;

// Set the default gamestate on file creation
void InitGameState();

// Update the state of the world each game tick
void UpdateGameState();

float GetDayIntensity();
float GetNightIntensity();
float GetDawnIntensity();
float GetDuskIntensity();

// Get the current sun/moon light color based on time of day
float GetSunIntensity();
Color GetSunColor();
Color GetSkyColor();
float GetFogDistance();
Color GetFogColor();

#endif