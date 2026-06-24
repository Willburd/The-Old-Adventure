#ifndef __ACTOR_TEXTBOX_HEADER__
#define __ACTOR_TEXTBOX_HEADER__

#include "raylib.h"
#include "actor.h"

struct Actor* TEXTBOX_CREATE(struct Actor* owner, struct Actor* player, char* text_id);

#endif