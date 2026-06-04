#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"
#include "assets.h"
#include "materials.h"

void scene_fieldtest_preloadassets(struct Actor* scene);
void scene_fieldtest_drawworld(struct Actor* scene, double tick_percent);

void scene_fieldtest_init(struct Actor* scene)
{
	// Configure scene
	scene->func_preloadassets = scene_fieldtest_preloadassets;
	scene->func_drawworld = scene_fieldtest_drawworld;

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);

	// Actor spawns
}

#define FIELD_ASSET_GROUND_TEXTURE ASSET_TEXTURES"/Objects/sign_wood.png"
#define FIELD_ASSET_MAIN_MODEL ASSET_MODELS"/Scenes/test_room.glb"

#define MAIN_MODEL_MESH_MAIN 0
#define MAIN_MODEL_MESH_COLLISION 1
#define MAIN_MODEL_MATERIAL_MAIN 0

void scene_fieldtest_preloadassets(struct Actor* scene)
{
	Asset* field_texture = LoadAsset_Texture(FIELD_ASSET_GROUND_TEXTURE, FALSE);
	Asset* model_asset = LoadAsset_Model(FIELD_ASSET_MAIN_MODEL, FALSE);
	SetActorCollision(scene, model_asset->mdl, MAIN_MODEL_MESH_COLLISION);
}

void scene_fieldtest_drawworld(struct Actor* scene, double tick_percent)
{
	Model* field_model = AssetGet_Model(FIELD_ASSET_MAIN_MODEL);
	Material* field_material = AssetGet_Material(ASSET_MATERIALS"/Error/no_material.mat");

	DrawMesh(field_model->meshes[MAIN_MODEL_MESH_MAIN], *field_material, GetMatrix(scene));
}