#include <stdlib.h>
#include "assets.h"
#include "materials.h"
#include "tools.h"

///////////////////////////////////////////////////////////////
//
// Materials are the primary system used to draw models.
// While it is possible to load textures and shaders, and apply
// them to a model by a dynamic material. Having a single loaded
// global version of the material, with shader arguments updated
// on each drawcall is much more efficient and sane.
// 
// Materials used globally through the whole game should be
// loaded directly in core_assets.c and flagged as a core asset.
// this means it will never be unloaded by scene transitions.
// always remember to update every shader param with intended values
// between each material draw. You don't know if another actor using
// this material left it in a state sane for your use!
// 
// Per actor materials can be used too, but must be recreated on each
// scene transition through the standard actor func_preloadassets.
// 
// Materials are special assets, materials are a combination of 
// shaders, textures, and parameters. This MUST be taken into account
// when creating a material, as materials will load these assets themselves
// if they are not already in the asset hashmap. Care should be taken to mark
// these assets as core when needed for reuse. Fear the material that has
// had it's textures or shader unloaded because they were not flagged properly.
// 
///////////////////////////////////////////////////////////////

Material LoadMaterial(char* path, int is_core_asset)
{
	// Setup material and read material define file
	Material mat = LoadMaterialDefault();


	// Get sub-assets needed to make the material function
	Texture2D* tex = LoadAsset_Texture("path", is_core_asset)->tex;
	Shader* shd = LoadAsset_Shader("id", "path_vert", "path_frag", is_core_asset)->shd;

	return mat;
}

void MaterialMapSet(Material* mat,int map_layer, float value, Color col, Texture2D* texture)
{
	mat->maps[map_layer].value = value;
	mat->maps[map_layer].color = col;
	SetMaterialTexture(mat, map_layer, *texture);
}


