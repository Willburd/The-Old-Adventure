#include "assets.h"
#include "tools.h"

#define MALLOC_ASSET(a, p) MALLOC(Asset, a);a->filepath = CHAR_STR_COPY(p);hashmap_set(loaded_assets, &a);

int asset_compare(const void* a, const void* b, void* udata) {
    const Asset* ua = a;
    const Asset* ub = b;
    return strcmp(ua->filepath, ub->filepath);
}

uint64_t asset_hash(const void* item, uint64_t seed0, uint64_t seed1) {
    const Asset* asset = item;
    return hashmap_sip(asset->filepath, strlen(asset->filepath), seed0, seed1);
}

void asset_free(void* item) {
    Asset* asset = item;
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
    Texture2D tex = LoadTexture(path);
    if (!IsTextureValid(tex))
    {
        // Failed asset load
        // TODO - Fallback asset
        return;
    }
    MALLOC_ASSET(asset, path);
    asset->tex = tex;
}

void LoadAsset_Model(const char* path)
{
    Model mdl = LoadModel(path);
    if (!IsModelValid(mdl))
    {
        // Failed asset load
        // TODO - Fallback asset
        return;
    }
    MALLOC_ASSET(asset, path);
    asset->mdl = mdl;
}

void LoadAsset_Sound(const char* path)
{
    Sound snd = LoadSound(path);
    if (!IsSoundValid(snd))
    {
        // Failed asset load
        // TODO - Fallback asset
        return;
    }
    MALLOC_ASSET(asset, path);
    asset->snd = snd;
}

void LoadAsset_Music(const char* path)
{
    Music mus = LoadMusicStream(path);
    if (!IsMusicValid(mus))
    {
        // Failed asset load
        // TODO - Fallback asset
        return;
    }
    MALLOC_ASSET(asset, path);
    asset->mus = mus;
}