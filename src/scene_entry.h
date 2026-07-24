#ifndef __SCENE_ENTRY_HEADER__
#define __SCENE_ENTRY_HEADER__

#include "stdint.h"
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
	ent_caveA,
	ent_caveB,
	ent_caveC,
	ent_caveD,
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

#define SCENE_CONFIG_TIMEPAUSED (1 << 0)
#define SCENE_CONFIG_HOTROOM	(1 << 1)
#define SCENE_CONFIG_COLDROOM	(1 << 2)
#define SCENE_CONFIG_ISRAINING	(1 << 3)

typedef struct {
	int active_room;
	uint64_t config_flags;	// Persistant settings assigned during scene creation or subroom entry
	uint64_t temp_flags;	// Temp flags set by actors in the scene to preserve state until the scene is unloaded.
	uint64_t perm_flags;	// Permanent flags set and loaded by the save file. So that state is preserved between scene reloads. 
	int utilityA1;
	int utilityA2;
	int utilityA3;
	int utilityA4;
	float utilityB1;
	float utilityB2;
	float utilityB3;
	float utilityB4;
} SceneData;

#define SCENEDATA_CLEAR(x) {SceneData* __dat = (SceneData*)x;__dat->active_room = 0;__dat->config_flags = 0;__dat->temp_flags = 0;__dat->perm_flags = 0;__dat->utilityA1 = 0;__dat->utilityA2 = 0;__dat->utilityA3 = 0;__dat->utilityA4 = 0;__dat->utilityB1 = 0.0f;__dat->utilityB2 = 0.0f;__dat->utilityB3 = 0.0f;__dat->utilityB4 = 0.0f;}

void LoadScene(SceneID id, EntranceID entrance);
void TransferScene(SceneID id, EntranceID entrance);
struct Actor* GetCurrentScene();
void HandleLoadNextScene();
void UnloadScene(int clear_assets);
void LoadSceneJSONActors(struct Actor* scene);

#endif