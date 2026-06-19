#include "gamestate.h"
#include "inventory.h"
#include "game_draw.h"
#include "actor_scene.h"

void InitGameState()
{
	// Start of game daynight cycle
	daynight_cycle = TIME_DAWN;
	daynight_speed = DEFAULT_DAYNIGHT_SPEED;

	// Setup inventory
	player_inventory.max_hearts = HEALTH_STARTING_HEARTS;
	player_inventory.health = player_inventory.max_hearts * HEALTH_PER_HEART;


}

void UpdateGameState()
{
	// Get the scenedata struct so we can get the currently set flags
	struct Actor* cur_scene = GetCurrentScene();
	uint64_t scene_config = 0;
	if (cur_scene != NULL && cur_scene->data != NULL)
	{
		SceneData* sdat = cur_scene->data;
		scene_config = sdat->config_flags;
	}
	// Cycle daynight if our scene doesn't pause time.
	if (!(scene_config & SCENE_CONFIG_TIMEPAUSED))
	{
		daynight_cycle += daynight_speed;
		while (daynight_cycle > 1.0f)
			daynight_cycle -= 1.0f;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
// Overworld rendering
//////////////////////////////////////////////////////////////////////////////////////////

float GetDayIntensity()
{
	return Clamp(sinf(daynight_cycle * (PI * 2.0f)) * 1.43f, 0.0f, 1.0f);
}

float GetNightIntensity()
{
	return Clamp(1.0f - sinf(daynight_cycle * (PI * 2.0f)), 0.0f, 1.0f);
}

#define SUNRISE_EXPONENT 40.0f // Higher exponent makes dusk/dawn shorter

float GetDawnIntensity()
{
	return powf(sinf((daynight_cycle + 0.5f) * PI), SUNRISE_EXPONENT);
}

float GetDuskIntensity()
{
	return powf((float)sinf(daynight_cycle * PI), SUNRISE_EXPONENT);
}

float GetSunIntensity()
{
	float night_intense = GetNightIntensity() * 0.32f;
	float day_intense = GetDayIntensity() * 0.96f;
	float dawn_intense = GetDawnIntensity() * 0.35f;
	float dusk_intense = GetDuskIntensity() * 0.30f;
	return Clamp(night_intense + day_intense + dawn_intense + dusk_intense, 0.0f, 1.0f);
}

Color GetSunColor()
{
	float day_intense = GetDayIntensity();
	float dawn_intense = GetDawnIntensity();
	float dusk_intense = GetDuskIntensity();

	Color col =				(Color) { 58, 54, 91, 255 };		// Night
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

	Color col =				(Color) { 46, 49, 66, 255 };	// Night
	col = ColorLerp(col,	(Color) { 126, 189, 252, 255 }, day_intense);  // Day
	col = ColorLerp(col,	(Color) { 138, 196, 255, 255 }, dawn_intense); // Dawn
	col = ColorLerp(col,	(Color) { 224, 195, 114, 255 }, dusk_intense); // Dusk
	return col;
}

float GetFogDistance()
{
	float day_intense = GetDayIntensity();
	float dawn_intense = GetDawnIntensity();
	float dusk_intense = GetDuskIntensity();
	return Lerp(FOG_DEFAULT_RANGE / 2.0f, FOG_DEFAULT_RANGE, Clamp(day_intense + dawn_intense + dusk_intense, 0.0f, 1.0f));
}

Color GetFogColor()
{
	float day_intense = GetDayIntensity();
	float dawn_intense = GetDawnIntensity();
	float dusk_intense = GetDuskIntensity();
	return ColorLerp(BLACK, GetSkyColor(), day_intense);
}