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
	ent_miscD,
	ent_graveA,
	ent_graveB,
	ent_graveC,
	ent_graveD,
	ent_caveA,
	ent_caveB,
	ent_caveC,
	ent_caveD,
	ent_checkpointA,
	ent_checkpointB,
	ent_checkpointC,
	ent_checkpointD,
	ent_checkpointE,
	ent_checkpointF,
	ent_checkpointG,
	ent_checkpointH,
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

#define ENTRANCE_STRING_CASE(str) if (STRMATCH(string_id, #str)) return ent_## str
inline EntranceID ENTRANCE_FROM_STRING(char* string_id)
{
	ENTRANCE_STRING_CASE(debugentrance);
	ENTRANCE_STRING_CASE(respawn);
	ENTRANCE_STRING_CASE(north);
	ENTRANCE_STRING_CASE(south);
	ENTRANCE_STRING_CASE(east);
	ENTRANCE_STRING_CASE(west);
	ENTRANCE_STRING_CASE(northwest);
	ENTRANCE_STRING_CASE(northeast);
	ENTRANCE_STRING_CASE(southwest);
	ENTRANCE_STRING_CASE(southeast);
	ENTRANCE_STRING_CASE(central);
	ENTRANCE_STRING_CASE(above);
	ENTRANCE_STRING_CASE(under);
	ENTRANCE_STRING_CASE(frontdoor);
	ENTRANCE_STRING_CASE(backdoor);
	ENTRANCE_STRING_CASE(attice);
	ENTRANCE_STRING_CASE(basement);
	ENTRANCE_STRING_CASE(bridge);
	ENTRANCE_STRING_CASE(underbridge);
	ENTRANCE_STRING_CASE(cliff);
	ENTRANCE_STRING_CASE(waterfall);
	ENTRANCE_STRING_CASE(slide);
	ENTRANCE_STRING_CASE(gate);
	ENTRANCE_STRING_CASE(portal);
	ENTRANCE_STRING_CASE(stairsA);
	ENTRANCE_STRING_CASE(stairsB);
	ENTRANCE_STRING_CASE(stairsC);
	ENTRANCE_STRING_CASE(stairsD);
	ENTRANCE_STRING_CASE(ladderA);
	ENTRANCE_STRING_CASE(ladderB);
	ENTRANCE_STRING_CASE(ladderC);
	ENTRANCE_STRING_CASE(ladderD);
	ENTRANCE_STRING_CASE(pitA);
	ENTRANCE_STRING_CASE(pitB);
	ENTRANCE_STRING_CASE(pitC);
	ENTRANCE_STRING_CASE(pitD);
	ENTRANCE_STRING_CASE(warpA);
	ENTRANCE_STRING_CASE(warpB);
	ENTRANCE_STRING_CASE(warpC);
	ENTRANCE_STRING_CASE(warpD);
	ENTRANCE_STRING_CASE(miscA);
	ENTRANCE_STRING_CASE(miscB);
	ENTRANCE_STRING_CASE(miscC);
	ENTRANCE_STRING_CASE(miscD);
	ENTRANCE_STRING_CASE(graveA);
	ENTRANCE_STRING_CASE(graveB);
	ENTRANCE_STRING_CASE(graveC);
	ENTRANCE_STRING_CASE(graveD);
	ENTRANCE_STRING_CASE(caveA);
	ENTRANCE_STRING_CASE(caveB);
	ENTRANCE_STRING_CASE(caveC);
	ENTRANCE_STRING_CASE(caveD);
	ENTRANCE_STRING_CASE(checkpointA);
	ENTRANCE_STRING_CASE(checkpointB);
	ENTRANCE_STRING_CASE(checkpointC);
	ENTRANCE_STRING_CASE(checkpointD);
	ENTRANCE_STRING_CASE(checkpointE);
	ENTRANCE_STRING_CASE(checkpointF);
	ENTRANCE_STRING_CASE(checkpointG);
	ENTRANCE_STRING_CASE(checkpointH);
	// NO_PLAYER_SCENE
	ENTRANCE_STRING_CASE(title);
	ENTRANCE_STRING_CASE(cutsceneA);
	ENTRANCE_STRING_CASE(cutsceneB);
	ENTRANCE_STRING_CASE(cutsceneC);
	ENTRANCE_STRING_CASE(cutsceneD);
	ENTRANCE_STRING_CASE(cutsceneE);
	ENTRANCE_STRING_CASE(cutsceneF);
	ENTRANCE_STRING_CASE(cutsceneG);
	ENTRANCE_STRING_CASE(cutsceneH);
	return LAST_ENTRANCE;
}
#undef ENTRANCE_STRING_CASE

#define SCENE_CONFIG_TIMEPAUSED (1 << 0)
#define SCENE_CONFIG_HOTROOM	(1 << 1)
#define SCENE_CONFIG_COLDROOM	(1 << 2)
#define SCENE_CONFIG_ISRAINING	(1 << 3)

typedef struct {
	uint64_t config_flags;	// Persistant settings assigned during scene creation or subroom entry
	uint64_t temp_flags;	// Temp flags set by actors in the scene to preserve state until the scene is unloaded.
	uint64_t perm_flags;	// Permanent flags set and loaded by the save file. So that state is preserved between scene reloads. 
	uint64_t puzzle_flags;	// Temp flags set by actors in the scene. Resets on room change instead of scene unload like temp_flags.
	// Misc utility vars, used for giving scenes quickly usable variables without needing custom data, or a dedicated flag.
	int utilityA1;
	int utilityA2;
	int utilityA3;
	int utilityA4;
	float utilityB1;
	float utilityB2;
	float utilityB3;
	float utilityB4;
} SceneData;

#define SCENEDATA_CLEAR(x) {SceneData* __dat = (SceneData*)x;__dat->config_flags = 0;__dat->temp_flags = 0;__dat->perm_flags = 0;__dat->puzzle_flags = 0;__dat->utilityA1 = 0;__dat->utilityA2 = 0;__dat->utilityA3 = 0;__dat->utilityA4 = 0;__dat->utilityB1 = 0.0f;__dat->utilityB2 = 0.0f;__dat->utilityB3 = 0.0f;__dat->utilityB4 = 0.0f;}

void LoadScene(SceneID id, EntranceID entrance);
void TransferScene(SceneID id, EntranceID entrance);
void ChangeSceneRoom(struct Actor* scene, int new_room_index, int keep_player);
struct Actor* GetCurrentScene();
void HandleLoadNextScene();
void UnloadScene(int clear_assets);
void LoadSceneJSONActors(struct Actor* scene);
void LoadCustomLayer(struct Actor* scene, char* custom_layer);

#endif