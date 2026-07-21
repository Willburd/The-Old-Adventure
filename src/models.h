#ifndef __MODELDATA_HEADER__
#define __MODELDATA_HEADER__

#include "raylib.h"
#include "cJSON.h"

typedef struct {
	char* mesh_name;
	int mesh_index;
} MeshInfo;

cJSON* ParseGLTFModel(char* path);
int GetMeshIndex(struct hashmap* mesh_data, char* mesh_name);

int meshdata_compare(const void* a, const void* b, void* udata);
uint64_t meshdata_hash(const void* item, uint64_t seed0, uint64_t seed1);
void meshdata_free(const void* item);

#endif