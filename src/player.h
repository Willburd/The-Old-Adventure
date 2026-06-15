#ifndef __ACTOR_PLAYER_HEADER__
#define __ACTOR_PLAYER_HEADER__

#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "input.h"
#include "camera.h"
#include "gamestate.h"
#include "collision.h"

#define PLAYER_COLLISION_MID_HEIGHT 0.50f
#define PLAYER_COLLISION_TOP_HEIGHT 1.0f
#define PLAYER_COLLISION_RADIUS 0.45f

#define PLAYER_TERMINAL_VELOCITY -1.6f

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
int PlayerCollisionEject(struct Actor* player, Vector3 start_offset, Vector3 dirvec, float radius);
int PlayerStandardRadialEjection(struct Actor* player, Vector3 start_offset, float radius);
void PlayerStandardHudDraw(struct Actor* player);

#endif