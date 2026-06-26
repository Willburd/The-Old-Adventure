#ifndef __GAMESTATE_HEADER__
#define __GAMESTATE_HEADER__

// Must match actor flags for gamestate
#define GAMESTATE_GAMEPLAY	(1 << 0)
#define GAMESTATE_TEXTBOX   (1 << 1)
#define GAMESTATE_TRANSITION (1 << 2)
#define GAMESTATE_CUTSCENE	(1 << 3)
#define GAMESTATE_PAUSED	(1 << 4)
// RESERVED (1 << 5)
// RESERVED (1 << 6)
// RESERVED (1 << 7)
#define GAMESTATE_FILTER_MODES 0xFF 

int gameplay_state;

#endif