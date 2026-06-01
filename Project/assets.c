#include "assets.h"
#include "tools.h"

#define MALLOC_ASSET(a) MALLOC(Asset, a);a->filepath = CHAR_STR_COPY(path);hashmap_set(loaded_assets, &a);

int asset_compare(const void* a, const void* b, void* udata) {
    const Asset* ua = a;
    const Asset* ub = b;
    return strcmp(ua->filepath, ub->filepath);
}

uint64_t asset_hash(const void* item, uint64_t seed0, uint64_t seed1) {
    const Asset* asset = item;
    return hashmap_sip(asset->filepath, strlen(asset->filepath), seed0, seed1);
}

void asset_free(const void* item) {
    const Asset* asset = item;
    if(IsTextureValid(asset->tex))
        UnloadTexture(asset->tex);
    if (IsModelValid(asset->mdl))
        UnloadModel(asset->mdl);
    if (IsSoundValid(asset->snd))
        UnloadSound(asset->snd);
    if (IsMusicValid(asset->mus))
        UnloadMusicStream(asset->mus);
    free(asset);
}

void LoadAsset_Texture(const char* path)
{
    MALLOC_ASSET(asset);
    asset->tex = LoadTexture(path);
}

void LoadAsset_Model(char* path)
{
    MALLOC_ASSET(asset);
    asset->mdl = LoadModel(path);
}

void LoadAsset_Sound(const char* path)
{
    MALLOC_ASSET(asset);
    asset->snd = LoadSound(path);
}

void LoadAsset_Music(const char* path)
{
    MALLOC_ASSET(asset);
    asset->mus = LoadMusicStream(path);
}