#ifndef __MODELDATA_HEADER__
#define __MODELDATA_HEADER__

#include "raylib.h"
#include "cJSON.h"

cJSON* ParseGLTFModel(char* path);
int GetMeshIndex(cJSON* model_json, char* mesh_name);

#endif