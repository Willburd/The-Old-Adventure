#include <stdlib.h>
#include <string.h>
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

#define PATH_LEN 100
Material LoadMaterial(char* path, int is_core_asset)
{
	// Setup material
	Material mat = LoadMaterialDefault();

	// Shader info
	char shader_vpath[PATH_LEN] = { 0 };
	char shader_fpath[PATH_LEN] = { 0 };

	// Load material define file
	FILE* fptr = fopen(path, "r");
	char cur_line[256];
	if (fptr == NULL) {
		printf("Asset: failed to open material definition: %s\n", path);
		return mat;
	}
	while (fgets(cur_line, 256, fptr)) {
		char identifier = cur_line[0];
		switch (identifier)
		{
		default:
		case '/': // Skip comment
			break;
		case '#': // Tag
			{
				// Step through string tokens
				char* next_token = "\0";
				char* tag_data = strtok_s(cur_line, " ", &next_token);
				while (tag_data != NULL) {
					// Remove \n chars and replace with endchar
					STRENDLINETERMINATE(tag_data);

					// Texture slot being used
					int map_type = MATERIAL_MAP_ALBEDO;
					if (STRMATCH(tag_data, "#ALBED")) map_type = MATERIAL_MAP_ALBEDO;
					if (STRMATCH(tag_data, "#METAL")) map_type = MATERIAL_MAP_METALNESS;
					if (STRMATCH(tag_data, "#NORMAL")) map_type = MATERIAL_MAP_NORMAL;
					if (STRMATCH(tag_data, "#ROUGH")) map_type = MATERIAL_MAP_ROUGHNESS;
					if (STRMATCH(tag_data, "#OCCLU")) map_type = MATERIAL_MAP_OCCLUSION;
					if (STRMATCH(tag_data, "#EMISS")) map_type = MATERIAL_MAP_EMISSION;
					if (STRMATCH(tag_data, "#HEIGH")) map_type = MATERIAL_MAP_HEIGHT;
					if (STRMATCH(tag_data, "#CUBEM")) map_type = MATERIAL_MAP_CUBEMAP;
					if (STRMATCH(tag_data, "#IRRAD")) map_type = MATERIAL_MAP_IRRADIANCE;
					if (STRMATCH(tag_data, "#PREFI")) map_type = MATERIAL_MAP_PREFILTER;
					if (STRMATCH(tag_data, "#BRDF_")) map_type = MATERIAL_MAP_BRDF;
					// Shader properties are collected and assigned at the end
					if (STRMATCH(tag_data, "#SHDRV"))
					{
						tag_data = strtok_s(NULL, " ", &next_token);
						if (tag_data == NULL)
							break;
						STRENDLINETERMINATE(tag_data);
						strcat_s(shader_vpath, PATH_LEN, tag_data);
						continue;
					}
					if (STRMATCH(tag_data, "#SHDRF"))
					{
						tag_data = strtok_s(NULL, " ", &next_token);
						if (tag_data == NULL)
							break;
						STRENDLINETERMINATE(tag_data);
						strcat_s(shader_fpath, PATH_LEN, tag_data);
						continue;
					}

					// Concat define and data from the material file to load textures
					tag_data = strtok_s(NULL, " ", &next_token);
					if (tag_data == NULL)
						break;
					STRENDLINETERMINATE(tag_data);
					Texture2D* tex = LoadAsset_Texture(TextFormat("%s%s", ASSET_TEXTURES, tag_data), is_core_asset)->tex;
					MaterialMapSet(&mat, map_type, 1.0f, WHITE, tex);
				}
				break;
			}
		}
	}
	fclose(fptr);

	// Set shader
	Shader shd = LoadShader(TextFormat("%s%s", ASSET_SHADERS, shader_vpath), TextFormat("%s%s", ASSET_SHADERS, shader_fpath));
	MaterialShaderSet(&mat, &shd);

	return mat;
}

void MaterialMapSet(Material* mat,int map_layer, float value, Color col, Texture2D* texture)
{
	mat->maps[map_layer].value = value;
	mat->maps[map_layer].color = col;
	SetMaterialTexture(mat, map_layer, *texture);
}

void MaterialShaderSet(Material* mat, Shader* shader)
{
	mat->shader = *shader;
}
