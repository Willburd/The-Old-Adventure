#ifndef __ACTOR_PLAYER_HEADER__
#define __ACTOR_PLAYER_HEADER__

#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "input.h"
#include "camera.h"
#include "gamestate.h"


enum PlayerState
{
	plysta_grounded,
	plysta_air,
};

typedef struct {
	int current_state;
	void (*func_state_update)(struct Actor* player);
	void (*func_state_drawworld)(struct Actor* player, double tick_percent);
	void (*func_state_drawhud)(struct Actor* player, double tick_percent);
	void (*func_state_exitstate)(struct Actor* player);
} PlayerData;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Player utility functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerChangeState(struct Actor* player, int new_state);
int PlayerCanAcceptInput(struct Actor* player);

#endif