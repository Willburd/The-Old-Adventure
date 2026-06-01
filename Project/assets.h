#ifndef ASSETS_HEADER
#define ASSETS_HEADER

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "raylib.h"
#include "hashmap.h"

#define ASSET_LIMIT 2048

struct hashmap* loaded_assets;

typedef struct {
    char* filepath;
    Texture2D tex;
    Model mdl;
    Sound snd;
    Music mus;
} Asset;

int asset_compare(const void* a, const void* b, void* udata);
uint64_t asset_hash(const void* item, uint64_t seed0, uint64_t seed1);
void asset_free(const void* item);

void LoadAsset_Texture(const char* path);
void LoadAsset_Model(const char* path);
void LoadAsset_Sound(const char* path);
void LoadAsset_Music(const char* path);

#endif
