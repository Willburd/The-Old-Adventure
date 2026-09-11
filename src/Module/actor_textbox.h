#ifndef __ACTOR_TEXTBOX_HEADER__
#define __ACTOR_TEXTBOX_HEADER__

#include "raylib.h"
#include "../actor.h"

#define TEXTBOX_DEFAULT_SIZE 16

#define TEXTBOX_DEFAULT_SPEED 0.6f

struct Actor* TEXTBOX_CREATE(struct Actor* owner, struct Actor* player, char* text_id, float text_speed);

#endif