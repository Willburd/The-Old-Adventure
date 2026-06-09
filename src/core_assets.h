#ifndef __CORE_ASSETS_HEADER__
#define __CORE_ASSETS_HEADER__

#include "raylib.h"
#include "assets.h"

#define SKYBOX_MATERIAL_CYCLE		ASSET_MATERIALS"/Skybox/sky_cycle.mat"

#define CUBEMAP_MODEL				ASSET_MODELS"/Tools/cube_map.glb"
#define SKYSPHERE_MODEL				ASSET_MODELS"/Tools/skysphere.glb"

void LoadCoreAssets();

#endif