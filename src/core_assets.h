#ifndef __CORE_ASSETS_HEADER__
#define __CORE_ASSETS_HEADER__

#include "raylib.h"
#include "assets.h"

#define SKYBOX_MATERIAL_CYCLE		ASSET_MATERIALS"/Skybox/sky_cycle.mat"

#define CUBEMAP_MODEL				ASSET_MODELS"/Tools/cube_map.glb"
#define SKYSPHERE_MODEL				ASSET_MODELS"/Tools/skysphere.glb"
#define QUAD_MODEL					ASSET_MODELS"/Tools/quad2d.glb"
#define SPRITE_MODEL				ASSET_MODELS"/Tools/sprite2d.glb"

#define PAUSEBOX_MODEL				ASSET_MODELS"/Tools/pausebox.glb"

void LoadCoreAssets();
void LoadCoreTextAssets();

// Text entries
#define TEXT_TEST_DEBUG "Test_Debug"
#define TEXT_ACTIONBUTTON_READ "ActionButton_Read"

#endif