#include "materials.h"

void MaterialMapSet(Material* mat, float value, Color col, Texture2D* texture)
{
	mat->maps[MATERIAL_MAP_ALBEDO].value = value;
	mat->maps[MATERIAL_MAP_ALBEDO].color = col;
	mat->maps[MATERIAL_MAP_ALBEDO].texture = *texture;
}


