#ifndef MODELS_HEADER
#define MODELS_HEADER

#include "raylib.h"

#define MAX_MATERIAL_MAPS 10

Material LoadMaterial(char* path);
void MaterialMapSet(Material* mat, int map_layer, float value, Color col, Texture2D* texture);

#endif