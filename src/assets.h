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

#define ASSET_LIMIT 2048

struct hashmap* loaded_assets;

typedef struct {
    char* filepath;
    Texture2D* tex;
    Model* mdl;
    Sound* snd;
    Music* mus;
} Asset;

void reset_global_asset_cache();

int asset_compare(const void* a, const void* b, void* udata);
uint64_t asset_hash(const void* item, uint64_t seed0, uint64_t seed1);
void asset_free(const void* item);

Asset* LoadAsset_Texture(char* path);
Asset* LoadAsset_Model(char* path);
Asset* LoadAsset_Sound(char* path);
Asset* LoadAsset_Music(char* path);
int AssetExists(char* path);
Asset* AssetGetPackage(char* path);

int AssetExists(char* path);
Texture2D* AssetGet_Texture(char* path);
Model* AssetGet_Model(char* path);
Sound* AssetGet_Sound(char* path);
Music* AssetGet_Music(char* path);
#endif
