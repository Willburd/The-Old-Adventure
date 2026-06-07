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
	return Clamp(sin(daynight_cycle * (PI * 2.0f)) * 1.43f, 0.0f, 1.0f);
}

float GetNightIntensity()
{
	return Clamp(1.0f - sin(daynight_cycle * (PI * 2.0f)), 0.0f, 1.0f);
}

float GetDawnIntensity()
{
	return pow(sin((daynight_cycle + 0.5f) * PI), 30.0f);
}

float GetDuskIntensity()
{
	return pow(sin(daynight_cycle * PI), 30.0f);
}

Color GetSunColor()
{
	float day_intense = GetDayIntensity();
	float night_intense = GetNightIntensity();
	float dawn_intense = GetDawnIntensity();
	float dusk_intense = GetDuskIntensity();

	Color col = BLACK;
	col = ColorLerp(col, WHITE, day_intense);
	col = ColorLerp(col, (Color){ 50, 50, 80, 255 }, night_intense);
	col = ColorLerp(col, (Color) { 40, 30, 80, 255 }, dawn_intense);
	col = ColorLerp(col, GOLD, dusk_intense);
	return col;
}