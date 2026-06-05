#ifndef MODELS_HEADER
#define MODELS_HEADER

#include "raylib.h"

Material LoadMaterial(Asset* asset, char* path, int is_core_asset);
void MaterialMapSet(Material* mat, int map_layer, float value, Color col, Texture2D* texture);
void MaterialShaderSet(Material* mat, Shader* shader);

#endif