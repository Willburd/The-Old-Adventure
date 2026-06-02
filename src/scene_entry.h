#ifndef SCENE_ENTRY_HEADER
#define SCENE_ENTRY_HEADER

typedef enum
{
	debugscene,
	bootscene,
	titlescene,
	testscene,
	LAST_SCENE

} SceneID;

const SceneID game_start_scene;

typedef enum
{
	debugentrance,
	titleentrance,
	respawn,
	north,
	south,
	east,
	west,
	northwest,
	northeast,
	southwest,
	southeast,
	central,
	above,
	under,
	frontdoor,
	backdoor,
	attice,
	basement,
	bridge,
	underbridge,
	cliff,
	waterfall,
	slide,
	gate,
	portal,
	stairsA,
	stairsB,
	stairsC,
	stairsD,
	ladderA,
	ladderB,
	ladderC,
	ladderD,
	pitA,
	pitB,
	pitC,
	pitD,
	warpA,
	warpB,
	warpC,
	warpD,
	miscA,
	miscB,
	miscC,
	misdD,
	graveA,
	graveB,
	graveC,
	graveD,
	NO_PLAYER_SCENE, // If beyond this point, don't spawn the player automatically
	cutsceneA,
	cutsceneB,
	cutsceneC,
	cutsceneD,
	cutsceneE,
	cutsceneF,
	cutsceneG,
	cutsceneH,
	LAST_ENTRANCE
} EntranceID;

#define SCENE_ENTRANCE_COUNT 20

typedef struct {
	int temp;
} SceneData;

void LoadScene(SceneID id, EntranceID entrance);

#endif