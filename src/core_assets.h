#ifndef __CORE_ASSETS_HEADER__
#define __CORE_ASSETS_HEADER__

#include "raylib.h"
#include "assets.h"

#define CUBEMAP_MODEL				ASSET_MODELS"/Tools/cube_map.glb"
#define QUAD_MODEL					ASSET_MODELS"/Tools/quad2d.glb"
#define SPRITE_MODEL				ASSET_MODELS"/Tools/sprite2d.glb"
#define CUBE_MODEL					ASSET_MODELS"/Tools/unit_cube.glb"
// Adventure
#define SIMPLE_ACTOR_COLLISION_MODEL					ASSET_MODELS"/Tools/simple_actor_collision.glb"

void LoadCoreAssets();
void LoadCoreTextAssets();

// Text entries
#define TEXT_TEST_DEBUG "Test_Debug"
// Adventure
#define TEXT_ACTIONBUTTON_READ "ActionButton_Read"

#endif