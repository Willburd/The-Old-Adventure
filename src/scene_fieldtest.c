#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"
#include "assets.h"
#include "models.h"
#include "materials.h"
#include "game_draw.h"
#include "actor_entrance.h"
#include "actor_trigger_exit.h"
#include "collision.h"
#include "world_state.h"
#include "light_tools.h"

// Assets
#define FIELD_MODEL ASSET_MODELS"/Scenes/test_room.glb"
#define FIELD_MATERIAL_MAIN ASSET_MATERIALS"/Objects/wood.mat"

// private header
SCENE_PRELOADASSETS(Sfieldtest);
SCENE_ACTIVATE_ROOM(Sfieldtest);
SCENE_LIGHTNODES(Sfieldtest);
SCENE_DRAWWORLD(Sfieldtest);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_INIT(Sfieldtest)
{
	// Configure scene
	SCENE_REGISTER_PRELOADASSETS(Sfieldtest);
	SCENE_REGISTER_LIGHTNODES(Sfieldtest);
	SCENE_REGISTER_ACTIVATE_ROOM(Sfieldtest);
	SCENE_REGISTER_DRAWWORLD(Sfieldtest);

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_PRELOADASSETS(Sfieldtest)
{
	// Load model
	Asset* model_asset = LoadAsset_Model(FIELD_MODEL, FALSE);
	LoadAsset_Material(FIELD_MATERIAL_MAIN, FALSE);

	// Set collision data
	REGISTER_COLLISION_MESH(scene, model_asset, "test_room", COL_LAYER_WORLD | COL_LAYER_CAMERA);
}

SCENE_ACTIVATE_ROOM(Sfieldtest)
{
	if ((rand() % 2) == 1)
	{
		SceneData* data = (SceneData*)scene->data;
		data->config_flags |= SCENE_CONFIG_ISRAINING;
		printf("Raining\n");
	}
}

SCENE_LIGHTNODES(Sfieldtest)
{
	LIGHT_NODE_CAVE(-1.5f, 0.15f, 36.0f, 15.0f);
}

SCENE_DRAWWORLD(Sfieldtest)
{
	Asset* model_asset = AssetGetPackage(FIELD_MODEL);
	STANDARD_SHADER_MATERIAL(field_mat, FIELD_MATERIAL_MAIN, scene);

	int main_mesh_index = GetMeshIndex(model_asset->mesh_data, "test_room");
	ToaDrawMesh(
		model_asset,
		main_mesh_index,
		*field_mat,
		GetMatrix(scene),
		FALSE
	);
}