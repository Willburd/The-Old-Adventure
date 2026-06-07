#include "gamestate.h"



void InitGameState()
{
	daynight_cycle = 0.2f;
	daynight_speed = DEFAULT_DAYNIGHT_SPEED;
}

void UpdateGameState()
{
	// Cycle daynight
	daynight_cycle += daynight_speed;
	while (daynight_cycle > 1.0f)
		daynight_cycle -= 1.0f;
}

float GetDayIntensity()
{
	return Clamp((float)sin(daynight_cycle * (PI * 2.0f)) * 1.43f, 0.0f, 1.0f);
}

float GetNightIntensity()
{
	return Clamp(1.0f - (float)sin(daynight_cycle * (PI * 2.0f)), 0.0f, 1.0f);
}

float GetDawnIntensity()
{
	return pow((float)sin((daynight_cycle + 0.5f) * PI), 30.0f);
}

float GetDuskIntensity()
{
	return pow((float)sin(daynight_cycle * PI), 30.0f);
}

Color GetSunColor()
{
	float day_intense = GetDayIntensity();
	float dawn_intense = GetDawnIntensity();
	float dusk_intense = GetDuskIntensity();

	Color col =				(Color) { 28, 24, 61, 255 };		// Night
	col = ColorLerp(col,	(Color) { 225, 237, 235, 255 }, day_intense);	// Day
	col = ColorLerp(col,	(Color) { 222, 167, 144, 255 }, dawn_intense); // Dawn
	col = ColorLerp(col,	(Color) { 227, 181, 52, 255 }, dusk_intense);	// Dusk
	return col;
}

Color GetSkyColor()
{
	float day_intense = GetDayIntensity();
	float dawn_intense = GetDawnIntensity();
	float dusk_intense = GetDuskIntensity();

	Color col =				(Color) { 26, 29, 46, 255 };	// Night
	col = ColorLerp(col,	(Color) { 126, 189, 252, 255 }, day_intense);  // Day
	col = ColorLerp(col,	(Color) { 138, 196, 255, 255 }, dawn_intense); // Dawn
	col = ColorLerp(col,	(Color) { 224, 195, 114, 255 }, dusk_intense); // Dusk
	return col;
}