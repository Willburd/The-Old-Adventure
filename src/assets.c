#include "assets.h"
#include "materials.h"
#include "tools.h"

// Remove all assets from the hashmap. Normally ignores core assets.
void UnloadAllAssets(int including_core)
{
    size_t iter = 0;
    void* item;
    // Scan the hashmap for assets that are not core assets
    while (hashmap_iter(loaded_assets, &iter, &item)) {
        const Asset* asset = item;
        if (!including_core && asset->core_asset)
            continue;
        hashmap_delete(loaded_assets, asset);
        asset_free(asset);
    }
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
        printf("ASSET: texture unloaded %s\n", asset->filepath);
    }
    if (asset->mdl != NULL)
    {
        UnloadModel(*asset->mdl);
        free(asset->mdl);
        printf("ASSET: model unloaded %s\n", asset->filepath);
    }
    if (asset->snd != NULL)
    {
        UnloadSound(*asset->snd);
        free(asset->snd);
        printf("ASSET: sound unloaded %s\n", asset->filepath);
    }
    if (asset->mus != NULL)
    {
        UnloadMusicStream(*asset->mus);
        free(asset->mus);
        printf("ASSET: music unloaded %s\n", asset->filepath);
    }
    if (asset->mat != NULL)
    {
        UnloadMaterial(*asset->mat);
        free(asset->mat);
        printf("ASSET: material unloaded %s\n", asset->filepath);
    }
    if (asset->shd != NULL)
    {
        UnloadShader(*asset->shd);
        free(asset->shd);
        printf("ASSET: shader unloaded %s\n", asset->filepath);
    }
    free(asset->filepath); // malloc char* string
}

#define RETURN_EXISTING_ASSET(pth) Asset* check = AssetGetPackage(pth); if (check){return check;}

Asset* LoadAsset_Texture(char* path)
{
    RETURN_EXISTING_ASSET(path);
    MALLOC_ASSET(asset, path);
    MALLOC_SET(Texture2D, asset->tex, FALSE);
    *asset->tex = LoadTexture(path);
    if (!IsTextureValid(*asset->tex))
        printf("ASSET: Unable to load texture: %s\n", path);
    hashmap_set(loaded_assets, asset);
    printf("ASSET: loaded texture: %s\n", path);
    return asset;
}

Asset* LoadAsset_Model(char* path)
{
    RETURN_EXISTING_ASSET(path);
    MALLOC_ASSET(asset, path);
    MALLOC_SET(Model, asset->mdl, FALSE);
    *asset->mdl = LoadModel(path);
    if (!IsModelValid(*asset->mdl))
        printf("ASSET: Unable to load model: %s\n", path);
    hashmap_set(loaded_assets, asset);
    printf("ASSET: loaded model: %s\n", path);
    return asset;
}

Asset* LoadAsset_Sound(char* path)
{
    RETURN_EXISTING_ASSET(path);
    MALLOC_ASSET(asset, path);
    MALLOC_SET(Sound, asset->snd, FALSE);
    *asset->snd = LoadSound(path);
    if (!IsSoundValid(*asset->snd))
        printf("ASSET: Unable to load sound: %s\n", path);
    hashmap_set(loaded_assets, asset);
    printf("ASSET: loaded sound: %s\n", path);
    return asset;
}

Asset* LoadAsset_Music(char* path)
{
    RETURN_EXISTING_ASSET(path);
    MALLOC_ASSET(asset, path);
    MALLOC_SET(Music, asset->mus, FALSE);
    *asset->mus = LoadMusicStream(path);
    if (!IsMusicValid(*asset->mus))
        printf("ASSET: Unable to load music: %s\n", path);
    hashmap_set(loaded_assets, asset);
    printf("ASSET: loaded music: %s\n", path);
    return asset;
}

Asset* LoadAsset_Material(char* path)
{
    RETURN_EXISTING_ASSET(path);
    MALLOC_ASSET(asset, path);
    MALLOC_SET(Material, asset->mat, FALSE);
    *asset->mat = LoadMaterial(path);
    if (!IsMaterialValid(*asset->mat))
        printf("ASSET: Unable to load material: %s\n", path);
    hashmap_set(loaded_assets, asset);
    printf("ASSET: loaded material: %s\n", path);
    return asset;
}

Asset* LoadAsset_Shader(char* shader_name, char* path_vertex, char* path_fragment)
{
    RETURN_EXISTING_ASSET(shader_name);
    MALLOC_ASSET(asset, shader_name);
    MALLOC_SET(Shader, asset->shd, FALSE);
    *asset->shd = LoadShader(path_vertex, path_fragment);
    if (!IsShaderValid(*asset->shd))
        printf("ASSET: Unable to load shader: %s\n", shader_name);
    hashmap_set(loaded_assets, asset);
    printf("ASSET: loaded shader: %s\n", shader_name);
    return asset;
}

int AssetExists(char* path)
{
    const Asset* asset = hashmap_get(loaded_assets, &(const Asset){.filepath = path });
    return asset == NULL ? FALSE : TRUE;
}

Asset* AssetGetPackage(char* path)
{
    return hashmap_get(loaded_assets, &(const Asset){.filepath = path });
}

#define ASSET_FALLBACK(orgpath,pth,ast) \
{\
    const Asset* asset = hashmap_get(loaded_assets, &(const Asset){.filepath = orgpath }); \
    if (asset == NULL) \
    { \
        const Asset* backup_asset = hashmap_get(loaded_assets, &(const Asset){.filepath = pth}); \
        return backup_asset->ast; \
    } \
    return asset->ast; \
}
Texture2D* AssetGet_Texture(char* path) ASSET_FALLBACK(path,ASSET_TEXTURES"/Error/no_texture.png", tex);
Model* AssetGet_Model(char* path) ASSET_FALLBACK(path, ASSET_TEXTURES"/Error/no_texture.png", mdl);
Sound* AssetGet_Sound(char* path) ASSET_FALLBACK(path, ASSET_TEXTURES"/Error/no_texture.png", snd);
Music* AssetGet_Music(char* path) ASSET_FALLBACK(path, ASSET_TEXTURES"/Error/no_texture.png", mus);
Material* AssetGet_Material(char* path) ASSET_FALLBACK(path, ASSET_TEXTURES"/Error/no_texture.png", mus);
Shader* AssetGet_Shader(char* shader_name) ASSET_FALLBACK(shader_name, ASSET_TEXTURES"/Error/no_texture.png", mus);
