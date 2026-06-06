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
#include "collision.h"

void scene_fieldtest_preloadassets(struct Actor* scene);
void scene_fieldtest_destroy(struct Actor* scene);
void scene_fieldtest_activate_room(struct Actor* scene, int room_index, int entrance);
void scene_fieldtest_lights(struct Actor* scene);
void scene_fieldtest_drawworld(struct Actor* scene, double tick_percent);

void scene_fieldtest_init(struct Actor* scene)
{
	// Configure scene
	scene->func_preloadassets = scene_fieldtest_preloadassets;
	scene->func_destroy = scene_fieldtest_destroy;
	scene->func_append_lights = scene_fieldtest_lights;
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
	// Load model
	Asset* model_asset = LoadAsset_Model(FIELD_ASSET_MAIN_MODEL, FALSE);
	LoadAsset_Material(FIELD_ASSET_MAIN_MATERIAL, FALSE);

	// Set collision data
	CollisionRegister(scene, &model_asset->mdl->meshes[MAIN_MODEL_MESH_COLLISION]);
}

void scene_fieldtest_destroy(struct Actor* scene)
{
	// clear collision data
	CollisionResign(scene, &AssetGet_Model(FIELD_ASSET_MAIN_MODEL)->meshes[MAIN_MODEL_MESH_COLLISION]);
}

void scene_fieldtest_activate_room(struct Actor* scene, int room_index, int entrance)
{
	// Store the current active subroom of the scene
	SceneData* data = (SceneData*)scene->data;
	data->active_room = room_index;

	// Entrances
	ENTRANCE_CREATE(ent_debugentrance, scene, (Vector3) { 0, 0, 2 }, (Vector3) { 1, 0, 2 });
	ENTRANCE_CREATE(ent_caveA, scene, (Vector3) { -0.2f, -1.1f, 21.5f }, (Vector3) { -0.2f, -1.1f, 19.0f });

	// Actor spawns
	ACTOR_FACTORY(act_skybox, scene, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
	
	// Set fog
	fog_set(SKYBLUE, FOG_DEFAULT_POWER, FOG_DEFAULT_RANGE);
}

void scene_fieldtest_lights(struct Actor* scene)
{
	lighting_append_light((Vector3) { 3.0, 2.0, 1.0 }, LIGHT_WORLD_RANGE, WHITE, 0.8f);
	/*
	lighting_append_light((Vector3) { 3.0, 2.0, 1.0 }, 30.0f, RED, 1.0f);
	lighting_append_light((Vector3) { 8.0, 1.0, 3.0 }, 30.0f, GREEN, 1.0f);
	*/
}

void scene_fieldtest_drawworld(struct Actor* scene, double tick_percent)
{
	Material* mat = AssetGet_Material(FIELD_ASSET_MAIN_MATERIAL);
	shader_update_camera_pos(mat->shader);
	shader_update_fog(mat->shader);
	shader_update_lights(mat->shader);

	DrawMesh(
		AssetGet_Model(FIELD_ASSET_MAIN_MODEL)->meshes[MAIN_MODEL_MESH_MAIN],
		*mat,
		GetMatrix(scene)
	);
}