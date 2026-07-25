#ifndef __WORLDSTATE_HEADER__
#define __WORLDSTATE_HEADER__

#include "tools.h"
#include "raylib.h"

#define DEFAULT_DAYNIGHT_SPEED 0.000085f;

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
float rain_intensity;

// Set the default gamestate on file creation
void InitWorldState();

// Update the state of the world each game tick
void UpdateWorldState();

float GetDayIntensity();
float GetNightIntensity();
float GetDawnIntensity();
float GetDuskIntensity();
float GetRainIntensity();

int IsDay();
int IsNight();

// Get the current sun/moon light color based on time of day
float GetSunIntensity();
Color GetSunColor();
Color GetSkyColor();
float GetFogDistance();
Color GetFogColor();

#endif