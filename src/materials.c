#include <stdlib.h>
#include "materials.h"

Material LoadMaterial(char* path)
{
	Material mat = LoadMaterialDefault();

	Texture2D tex;

	Shader shd;

	return mat;
}

void MaterialMapSet(Material* mat,int map_layer, float value, Color col, Texture2D* texture)
{
	mat->maps[map_layer].value = value;
	mat->maps[map_layer].color = col;
	SetMaterialTexture(mat, map_layer, *texture);
}


