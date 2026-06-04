#ifndef ASSETS_HEADER
#define ASSETS_HEADER

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "raylib.h"
#include "hashmap.h"

// TODO - Properly set something up for this
#ifdef _RELEASE
#define ASSET_PATH "./Assets"
#else
#define ASSET_PATH "../Assets"
#endif

#define ASSET_TEXTURES ASSET_PATH"/Textures"
#define ASSET_MODELS ASSET_PATH"/Models"
#define ASSET_MATERIALS ASSET_PATH"/Materials"
#define ASSET_SHADERS ASSET_PATH"/Shaders"

#define ASSET_LIMIT 2048

struct hashmap* loaded_assets;

typedef struct {
    int core_asset;
    char* filepath;
    Texture2D* tex;
    Model* mdl;
    Sound* snd;
    Music* mus;
    Material* mat;
} Asset;

#define MALLOC_ASSET(a, p, s_core) MALLOC(Asset, a, 0);a->core_asset=s_core;CHAR_STR_COPY(a->filepath, p, 0);a->tex=NULL;a->mdl=NULL;a->snd=NULL;a->mus=NULL;a->mat=NULL;

void UnloadAllAssets(int including_core);

int asset_compare(const void* a, const void* b, void* udata);
uint64_t asset_hash(const void* item, uint64_t seed0, uint64_t seed1);
void asset_free(const void* item);

Asset* LoadAsset_Texture(char* path, int is_core);
Asset* LoadAsset_Model(char* path, int is_core);
Asset* LoadAsset_Sound(char* path, int is_core);
Asset* LoadAsset_Music(char* path, int is_core);
Asset* LoadAsset_Material(char* path, int is_core);
int AssetExists(char* path);
Asset* AssetGetPackage(char* path);

int AssetExists(char* path);
Texture2D* AssetGet_Texture(char* path);
Model* AssetGet_Model(char* path);
Sound* AssetGet_Sound(char* path);
Music* AssetGet_Music(char* path);
Material* AssetGet_Material(char* path);
#endif
