#ifndef GAMESTATE_HEADER
#define GAMESTATE_HEADER

#include "tools.h"
#include "raylib.h"

#define DEFAULT_DAYNIGHT_SPEED 0.00075f;

// 0 dawn, 0.25 midday, 0.5 dusk, 0.75 midnight, 1 dawn
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
Color GetSunColor();
Color GetSkyColor();

#endif