#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"
#include "assets.h"

void scene_fieldtest_preloadassets(struct Actor* scene);
void scene_fieldtest_destroy(struct Actor* scene);
void scene_fieldtest_drawworld(struct Actor* scene, double tick_percent);

void scene_fieldtest_init(struct Actor* scene)
{
	// Configure scene
	scene->func_preloadassets = scene_fieldtest_preloadassets;
	scene->func_drawworld = scene_fieldtest_drawworld;
	scene->func_destroy = scene_fieldtest_destroy;

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);

	// Actor spawns
}

#define FIELD_ASSET_GROUND_TEXTURE ASSET_TEXTURES"/Objects/sign_wood.png"
#define FIELD_ASSET_MAIN_MODEL ASSET_MODELS"/Scenes/test_room.blend"

void scene_fieldtest_preloadassets(struct Actor* scene)
{
	LoadAsset_Texture(FIELD_ASSET_GROUND_TEXTURE);
	Asset* asset_model = LoadAsset_Model(FIELD_ASSET_MAIN_MODEL);
}


void scene_fieldtest_drawworld(struct Actor* scene, double tick_percent)
{

}

void scene_fieldtest_destroy(struct Actor* scene)
{

}
