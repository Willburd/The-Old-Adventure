#include "assets.h"
#include "tools.h"

#define MALLOC_ASSET(a, p) MALLOC(Asset, a);CHAR_STR_COPY(a->filepath, p);hashmap_set(loaded_assets, a);

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
    free(asset->filepath); // malloc char* string
    free(asset);
}

Texture2D LoadAsset_Texture(const char* path)
{
    Texture2D tex = LoadTexture(path);
    if (!IsTextureValid(tex))
    {
        printf("Unable to load asset: %s", path);
        // Failed asset load
        // TODO - Fallback asset
        return;
    }
    MALLOC_ASSET(asset, path);
    asset->tex = tex;
    return asset->tex;
}

Model LoadAsset_Model(const char* path)
{
    Model mdl = LoadModel(path);
    if (!IsModelValid(mdl))
    {
        printf("Unable to load asset: %s", path);
        // Failed asset load
        // TODO - Fallback asset
        return;
    }
    MALLOC_ASSET(asset, path);
    asset->mdl = mdl;
    return asset->mdl;
}

Sound LoadAsset_Sound(const const char* path)
{
    Sound snd = LoadSound(path);
    if (!IsSoundValid(snd))
    {
        printf("Unable to load asset: %s", path);
        // Failed asset load
        // TODO - Fallback asset
        return;
    }
    MALLOC_ASSET(asset, path);
    asset->snd = snd;
    return asset->snd;
}

Music LoadAsset_Music(const char* path)
{
    Music mus = LoadMusicStream(path);
    if (!IsMusicValid(mus))
    {
        printf("Unable to load asset: %s", path);
        // Failed asset load
        // TODO - Fallback asset
        return;
    }
    MALLOC_ASSET(asset, path);
    asset->mus = mus;
    return asset->mus;
}


Texture2D AssetGet_Texture(const char* path)
{
    Asset* asset = hashmap_get(loaded_assets, &(Asset){.filepath = path });
    return asset->tex;
}

Model AssetGet_Model(const char* path)
{
    Asset* asset = hashmap_get(loaded_assets, &(Asset){.filepath = path });
    return asset->mdl;
}

Sound AssetGet_Sound(const char* path)
{
    Asset* asset = hashmap_get(loaded_assets, &(Asset){.filepath = path });
    return asset->snd;
}

Music AssetGet_Music(const char* path)
{
    Asset* asset = hashmap_get(loaded_assets, &(Asset){.filepath = path });
    return asset->mus;
}
