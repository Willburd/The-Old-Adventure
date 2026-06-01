#ifndef ASSETS_HEADER
#define ASSETS_HEADER

#include <string.h>
#include <stdio.h>
#include "globals.h"
#include "raylib.h"
#include "hashmap.h"

#define ASSET_LIMIT 2048

struct hashmap* loaded_assets;

typedef struct {
    char* filepath;
    int age;
    Texture2D tex;
    Model mdl;
    Sound snd;
    Music mus;
} Asset;

int asset_compare(const void* a, const void* b, void* udata);
int asset_iter(const void* item, void* udata);
__int64 asset_hash(const void* item, __int64 seed0, __int64 seed1);
void asset_free(const void* item);

void LoadAsset_Texture(const char* path);
void LoadAsset_Model(const char* path);
void LoadAsset_Sound(const char* path);
void LoadAsset_Music(const char* path);

#endif
