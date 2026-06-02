#include "assets.h"
#include "tools.h"

#define MALLOC_ASSET(a, p) MALLOC(Asset, a, 0);CHAR_STR_COPY(a->filepath, p, 0);a->tex=NULL;a->mdl=NULL;a->snd=NULL;a->mus=NULL;

// Clears all loaded assets and reloads the global base assets only.
void reset_global_asset_cache()
{
    // Wipe the current asset cache
    hashmap_clear(loaded_assets, FALSE);

    // Load the default assets
    LoadAsset_Texture(ASSET_TEXTURES"/Error/no_texture.png");
    LoadAsset_Texture(ASSET_TEXTURES"/Error/no_material.png");
}

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
    if (asset->tex != NULL)
    {
        UnloadTexture(*asset->tex);
        free(asset->tex);
    }
    if (asset->mdl != NULL)
    {
        UnloadModel(*asset->mdl);
        free(asset->mdl);
    }
    if (asset->snd != NULL)
    {
        UnloadSound(*asset->snd);
        free(asset->snd);
    }
    if (asset->mus != NULL)
    {
        UnloadMusicStream(*asset->mus);
        free(asset->mus);
    }
    free(asset->filepath); // malloc char* string
}

int LoadAsset_Texture(char* path)
{
    if (AssetExists(path))
        return FALSE;
    MALLOC_ASSET(asset, path);
    MALLOC_SET(Texture2D, asset->tex, FALSE);
    *asset->tex = LoadTexture(path);
    if (!IsTextureValid(*asset->tex))
        printf("Unable to load asset: %s", path);
    hashmap_set(loaded_assets, asset);
    return TRUE;
}

int LoadAsset_Model(char* path)
{
    if (AssetExists(path))
        return FALSE;
    MALLOC_ASSET(asset, path);
    MALLOC_SET(Model, asset->mdl, FALSE);
    *asset->mdl = LoadModel(path);
    if (!IsModelValid(*asset->mdl))
        printf("Unable to load asset: %s", path);
    hashmap_set(loaded_assets, asset);
    return TRUE;
}

int LoadAsset_Sound(char* path)
{
    if (AssetExists(path))
        return FALSE;
    MALLOC_ASSET(asset, path);
    MALLOC_SET(Sound, asset->snd, FALSE);
    *asset->snd = LoadSound(path);
    if (!IsSoundValid(*asset->snd))
        printf("Unable to load asset: %s", path);
    hashmap_set(loaded_assets, asset);
    return TRUE;
}

int LoadAsset_Music(char* path)
{
    if (AssetExists(path))
        return FALSE;
    MALLOC_ASSET(asset, path);
    MALLOC_SET(Music, asset->mus, FALSE);
    *asset->mus = LoadMusicStream(path);
    if (!IsMusicValid(*asset->mus))
        printf("Unable to load asset: %s", path);
    hashmap_set(loaded_assets, asset);
    return TRUE;
}

int AssetExists(char* path)
{
    const Asset* asset = hashmap_get(loaded_assets, &(const Asset){.filepath = path });
    return asset == NULL ? FALSE : TRUE;
}

#define ASSET_FALLBACK(orgpath,pth,ast) \
{\
    const Asset* asset = hashmap_get(loaded_assets, &(const Asset){.filepath = orgpath }); \
    if (asset == NULL) \
    { \
        const Asset* backup_asset = hashmap_get(loaded_assets, &(const Asset){.filepath = pth}); \
        return *backup_asset->ast; \
    } \
    return *asset->ast; \
}
Texture2D AssetGet_Texture(char* path) ASSET_FALLBACK(path,ASSET_TEXTURES"/Error/no_texture.png", tex);
Model AssetGet_Model(char* path) ASSET_FALLBACK(path, ASSET_TEXTURES"/Error/no_texture.png", mdl);
Sound AssetGet_Sound(char* path) ASSET_FALLBACK(path, ASSET_TEXTURES"/Error/no_texture.png", snd);
Music AssetGet_Music(char* path) ASSET_FALLBACK(path, ASSET_TEXTURES"/Error/no_texture.png", mus);
