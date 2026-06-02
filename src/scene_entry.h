#ifndef SCENE_ENTRY_HEADER
#define SCENE_ENTRY_HEADER

#include "scene_library.h"

typedef enum
{
	ent_debugentrance,
	ent_respawn,
	ent_north,
	ent_south,
	ent_east,
	ent_west,
	ent_northwest,
	ent_northeast,
	ent_southwest,
	ent_southeast,
	ent_central,
	ent_above,
	ent_under,
	ent_frontdoor,
	ent_backdoor,
	ent_attice,
	ent_basement,
	ent_bridge,
	ent_underbridge,
	ent_cliff,
	ent_waterfall,
	ent_slide,
	ent_gate,
	ent_portal,
	ent_stairsA,
	ent_stairsB,
	ent_stairsC,
	ent_stairsD,
	ent_ladderA,
	ent_ladderB,
	ent_ladderC,
	ent_ladderD,
	ent_pitA,
	ent_pitB,
	ent_pitC,
	ent_pitD,
	ent_warpA,
	ent_warpB,
	ent_warpC,
	ent_warpD,
	ent_miscA,
	ent_miscB,
	ent_miscC,
	ent_misdD,
	ent_graveA,
	ent_graveB,
	ent_graveC,
	ent_graveD,
	NO_PLAYER_SCENE, // If beyond this point, don't spawn the player automatically
	ent_title,
	ent_cutsceneA,
	ent_cutsceneB,
	ent_cutsceneC,
	ent_cutsceneD,
	ent_cutsceneE,
	ent_cutsceneF,
	ent_cutsceneG,
	ent_cutsceneH,
	LAST_ENTRANCE
} EntranceID;

#define SCENE_ENTRANCE_COUNT 20

typedef struct {
	int temp;
} SceneData;

void LoadScene(SceneID id, EntranceID entrance, int unload_previous);
void ReloadScene();
void UnloadScene();

#endif