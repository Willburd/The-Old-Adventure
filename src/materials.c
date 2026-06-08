#include <stdlib.h>
#include <string.h>
#include "assets.h"
#include "materials.h"
#include "tools.h"

// Step through the current line's tokens
#define ADVANCETOKEN(tag) tag = strtok_s(NULL, " ", &next_token);if (tag == NULL){break;};STRENDLINETERMINATE(tag);
#define PATH_LEN 100

static void MaterialMapSet(Material * mat, int map_layer, float value, Color col, Texture2D * texture);
static void MaterialShaderSet(Material * mat, Shader * shader);

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
// Shaders are special snowflakes. They are not tracked by the asset
// loader, and instead by raylib itself. They are tied to each material
// Materials will unload their shader with them.
// 
///////////////////////////////////////////////////////////////

Material LoadMaterial(Asset* asset, char* path, int is_core_asset)
{
	// Setup material
	Material mat = LoadMaterialDefault();

	// Load material define file
	FILE* fptr = fopen(path, "r");
	char cur_line[256];
	if (fptr == NULL) {
		printf("Asset: failed to open material definition: %s\n", path);
		return mat;
	}
	while (fgets(cur_line, 256, fptr)) {
		// Ignore everything except actual material defines
		if (cur_line[0] != '#')
			continue;

		// Step through string tokens
		char* next_token = "\0";
		char* tag_data = strtok_s(cur_line, " ", &next_token);
		while (tag_data != NULL) {
			// Remove \n chars and replace with endchar
			STRENDLINETERMINATE(tag_data);

			// Shader being used
			if (STRMATCH(tag_data, "#SHADR"))
			{
				// Set shader
				char shader_vpath[PATH_LEN] = { 0 };
				char shader_fpath[PATH_LEN] = { 0 };

				ADVANCETOKEN(tag_data);
				strcat_s(shader_vpath, PATH_LEN, tag_data);

				ADVANCETOKEN(tag_data);
				strcat_s(shader_fpath, PATH_LEN, tag_data);

				Shader shd = LoadShader(TextFormat("%s%s", ASSET_SHADERS, shader_vpath), TextFormat("%s%s", ASSET_SHADERS, shader_fpath));
				MaterialShaderSet(&mat, &shd);
				continue;
			}

			// Texture slot being used
			int map_type = MATERIAL_MAP_ALBEDO;
			if (STRMATCH(tag_data, "#ALBED") || STRMATCH(tag_data, "#TEX00")) map_type = MATERIAL_MAP_ALBEDO;
			if (STRMATCH(tag_data, "#METAL") || STRMATCH(tag_data, "#TEX01")) map_type = MATERIAL_MAP_METALNESS;
			if (STRMATCH(tag_data, "#NORMAL")|| STRMATCH(tag_data, "#TEX02")) map_type = MATERIAL_MAP_NORMAL;
			/* Inaccessible normally
			if (STRMATCH(tag_data, "#ROUGH") || STRMATCH(tag_data, "#TEX03")) map_type = MATERIAL_MAP_ROUGHNESS;
			if (STRMATCH(tag_data, "#OCCLU") || STRMATCH(tag_data, "#TEX04")) map_type = MATERIAL_MAP_OCCLUSION;
			if (STRMATCH(tag_data, "#EMISS") || STRMATCH(tag_data, "#TEX05")) map_type = MATERIAL_MAP_EMISSION;
			if (STRMATCH(tag_data, "#HEIGH") || STRMATCH(tag_data, "#TEX06")) map_type = MATERIAL_MAP_HEIGHT;
			if (STRMATCH(tag_data, "#CUBEM") || STRMATCH(tag_data, "#TEX07")) map_type = MATERIAL_MAP_CUBEMAP;
			if (STRMATCH(tag_data, "#IRRAD") || STRMATCH(tag_data, "#TEX08")) map_type = MATERIAL_MAP_IRRADIANCE;
			if (STRMATCH(tag_data, "#PREFI") || STRMATCH(tag_data, "#TEX09")) map_type = MATERIAL_MAP_PREFILTER;
			if (STRMATCH(tag_data, "#BRDF_") || STRMATCH(tag_data, "#TEX10")) map_type = MATERIAL_MAP_BRDF;
			*/
			
			// Creature texture and link it to the material
			ADVANCETOKEN(tag_data);
			Texture2D* tex = LoadAsset_Texture(TextFormat("%s%s", ASSET_TEXTURES, tag_data), is_core_asset, path)->tex;
			MaterialMapSet(&mat, map_type, 1.0f, WHITE, tex);

			// Additional texture properties
			ADVANCETOKEN(tag_data); // Filtering
			if (STRMATCH(tag_data, "FILT_POINT")) SetTextureFilter(*tex, TEXTURE_FILTER_POINT);
			if (STRMATCH(tag_data, "FILT_BI")) SetTextureFilter(*tex, TEXTURE_FILTER_BILINEAR);
			if (STRMATCH(tag_data, "FILT_TRI")) SetTextureFilter(*tex, TEXTURE_FILTER_TRILINEAR);

			ADVANCETOKEN(tag_data); // Wrapping
			if (STRMATCH(tag_data, "WRAP_REPEAT")) SetTextureWrap(*tex, TEXTURE_WRAP_REPEAT);
			if (STRMATCH(tag_data, "WRAP_CLAMP")) SetTextureWrap(*tex, TEXTURE_WRAP_CLAMP);
			if (STRMATCH(tag_data, "WRAP_MIRROR")) SetTextureWrap(*tex, TEXTURE_WRAP_MIRROR_REPEAT);
			if (STRMATCH(tag_data, "WRAP_CLAMP_MIRROR")) SetTextureWrap(*tex, TEXTURE_WRAP_MIRROR_CLAMP);
		}
	}
	fclose(fptr);
	return mat;
}

static void MaterialMapSet(Material* mat,int map_layer, float value, Color col, Texture2D* texture)
{
	mat->maps[map_layer].value = value;
	mat->maps[map_layer].color = col;
	SetMaterialTexture(mat, map_layer, *texture);
}

static void MaterialShaderSet(Material* mat, Shader* shader)
{
	mat->shader = *shader;
}
