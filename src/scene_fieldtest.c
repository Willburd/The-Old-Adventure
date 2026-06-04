#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"
#include "assets.h"
#include "materials.h"
#include "game_draw.h"
#include "actor_entrance.h"

void scene_fieldtest_preloadassets(struct Actor* scene);
void scene_fieldtest_activate_room(struct Actor* scene, int room_index, EntranceID entrance);
void scene_fieldtest_drawworld(struct Actor* scene, double tick_percent);

void scene_fieldtest_init(struct Actor* scene)
{
	// Configure scene
	scene->func_preloadassets = scene_fieldtest_preloadassets;
	scene->func_activate_room = scene_fieldtest_activate_room;
	scene->func_drawworld = scene_fieldtest_drawworld;

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);

	// Set scene background
	clear_background_color = SKYBLUE;
}

#define FIELD_ASSET_MAIN_MODEL ASSET_MODELS"/Scenes/test_room.glb"
#define FIELD_ASSET_MAIN_MATERIAL ASSET_MATERIALS"/Objects/wood.mat"

#define MAIN_MODEL_MESH_MAIN 0
#define MAIN_MODEL_MESH_COLLISION 1
#define MAIN_MODEL_MATERIAL_MAIN 0

void scene_fieldtest_preloadassets(struct Actor* scene)
{
	Asset* model_asset = LoadAsset_Model(FIELD_ASSET_MAIN_MODEL, FALSE);
	SetActorCollision(scene, model_asset->mdl, MAIN_MODEL_MESH_COLLISION);
	LoadAsset_Material(FIELD_ASSET_MAIN_MATERIAL, FALSE);
}

void scene_fieldtest_activate_room(struct Actor* scene, int room_index, EntranceID entrance)
{
	// Store the current active subroom of the scene
	SceneData* data = (SceneData*)scene->data;
	data->active_room = room_index;

	// Entrances
	entrance_create(ent_debugentrance, scene, (Vector3) { 0, 0, 2 }, (Vector3) { 1, 0, 2 });
	entrance_create(ent_caveA, scene, (Vector3) { -0.2, -1.1, 21.5 }, (Vector3) { -0.2, -1.1, 19.0 });

	// Actor spawns

}

void scene_fieldtest_drawworld(struct Actor* scene, double tick_percent)
{
	DrawMesh(
		AssetGet_Model(FIELD_ASSET_MAIN_MODEL)->meshes[MAIN_MODEL_MESH_MAIN],
		*AssetGet_Material(FIELD_ASSET_MAIN_MATERIAL),
		GetMatrix(scene)
	);
}