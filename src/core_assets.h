#ifndef CORE_ASSETS_HEADER
#define CORE_ASSETS_HEADER

#include "raylib.h"
#include "assets.h"


#define SKYBOX_MATERIAL_DAWN		ASSET_MATERIALS"/Skybox/sky_dawn.mat"
#define SKYBOX_MATERIAL_DAY			ASSET_MATERIALS"/Skybox/sky_day.mat"
#define SKYBOX_MATERIAL_DAYRAIN		ASSET_MATERIALS"/Skybox/sky_dayrain.mat"
#define SKYBOX_MATERIAL_DUSK		ASSET_MATERIALS"/Skybox/sky_dusk.mat"
#define SKYBOX_MATERIAL_NIGHT		ASSET_MATERIALS"/Skybox/sky_night.mat"
#define SKYBOX_MATERIAL_NIGHTRAIN	ASSET_MATERIALS"/Skybox/sky_nightrain.mat"

#define SKYBOX_MATERIAL_CYCLE		ASSET_MATERIALS"/Skybox/sky_cycle.mat"

#define CUBEMAP_MODEL				ASSET_MODELS"/Tools/cube_map.glb"
#define SKYSPHERE_MODEL				ASSET_MODELS"/Tools/skysphere.glb"

void LoadCoreAssets();

#endif