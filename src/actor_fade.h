#ifndef __ACTOR_FADEIN_HEADER__
#define __ACTOR_FADEIN_HEADER__

#include "raylib.h"
#include "actor.h"

#define FADE_RATE 5

typedef struct
{
	Color blend_color;
	int fadeout;
	int previous_fadeout;
} FadeInData;

struct Actor* FADEIN_CREATE(Color color);
struct Actor* FADEOUT_CREATE(Color color);

#endif