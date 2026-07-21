#include <stdlib.h>
#include <string.h>
#include "assets.h"
#include "animation.h"
#include "materials.h"
#include "models.h"
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
    if (including_core)
    {
        UnloadFont(default_font);
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

void asset_free(void* item) {
    Asset* asset = item;
    if (asset->tex != NULL && IsTextureValid(*asset->tex))
    {
        UnloadTexture(*asset->tex);
        RELEASE(asset->tex);
        printf("ASSET: texture unloaded %s\n", asset->filepath);
    }
    if (asset->mdl != NULL && IsModelValid(*asset->mdl))
    {
        // Unload anims
        if (asset->anm != NULL && IsModelAnimationValid(*asset->mdl, *asset->anm))
        {
            UnloadModelAnimations(asset->anm, asset->anm_count);
            asset->anm = NULL; // Do not free, UnloadModelAnimations above does that.
            asset->anm_count = 0;
            printf("ASSET: animations unloaded %s\n", asset->filepath);
        }
        // Unload model
        UnloadModel(*asset->mdl);
        RELEASE(asset->mdl);
        // Unload mesh lookup data
        hashmap_free(asset->mesh_data);
        asset->mesh_data = NULL;
        printf("ASSET: model unloaded %s\n", asset->filepath);
    }
    if (asset->snd != NULL && IsSoundValid(*asset->snd))
    {
        UnloadSound(*asset->snd);
        RELEASE(asset->snd);
        printf("ASSET: sound unloaded %s\n", asset->filepath);
    }
    if (asset->mus != NULL && IsMusicValid(*asset->mus))
    {
        UnloadMusicStream(*asset->mus);
        RELEASE(asset->mus);
        printf("ASSET: music unloaded %s\n", asset->filepath);
    }
    if (asset->mat != NULL && IsMaterialValid(*asset->mat))
    {
        // Unload material's local textures

        // Unload map
        UnloadMaterial(*asset->mat);
        RELEASE(asset->mat);
        printf("ASSET: material unloaded %s\n", asset->filepath);
    }
    RELEASE(asset->filepath); // malloc char* string
}

#define RETURN_EXISTING_ASSET(pth, s_core) Asset* check = AssetGetPackage(pth); if (check){if(s_core){check->core_asset=s_core;};return check;}

Asset* LoadAsset_Texture(char* path, int is_core, char* mat_link)
{
    RETURN_EXISTING_ASSET(path, is_core);
    char* tex_name = mat_link != NULL ? TextFormat("%s[%s]", mat_link, path) : path;
    MALLOC_ASSET(asset, tex_name, is_core);
    MALLOC_SET(Texture2D, asset->tex, FALSE);
    *asset->tex = LoadTexture(path);
    if (!IsTextureValid(*asset->tex))
        printf("ASSET: Unable to load texture: %s\n", path);
    // This uses MEMSET, ensure all data is assigned before hashmapping!
    hashmap_set(loaded_assets, asset);
    if(mat_link != NULL)
        printf("ASSET: loaded material-texture: %s\n", asset->filepath);
    else
        printf("ASSET: loaded texture: %s\n", asset->filepath);
    return asset;
}

Asset* LoadAsset_Model(char* path, int is_core)
{
    RETURN_EXISTING_ASSET(path, is_core);
    MALLOC_ASSET(asset, path, is_core);
    MALLOC_SET(Model, asset->mdl, FALSE);
    // Load model
    *asset->mdl = LoadModel(path);
    // Get the model's mesh data
    printf("ASSET: loading model meshes: %s\n", asset->filepath);
    int mesh_load_index = 0;
    cJSON* model_json = ParseGLTFModel(path);
    cJSON* node_array = cJSON_GetObjectItem(model_json, "nodes");
    cJSON* material_array = cJSON_GetObjectItem(model_json, "materials");
    cJSON* mesh_array = cJSON_GetObjectItem(model_json, "meshes");
    asset->mesh_data = hashmap_new(sizeof(MeshInfo), 64, 0, 0, meshdata_hash, meshdata_compare, meshdata_free, NULL);
    for (int i = 0; i < cJSON_GetArraySize(node_array); i++)
    {
        // For each node...
        cJSON* node_entry = cJSON_GetArrayItem(node_array, i);
        char* node_data_name = cJSON_GetObjectItem(node_entry, "name")->valuestring;
        int node_data_meshindex = cJSON_GetObjectItem(node_entry, "mesh")->valueint;

        // Check each mesh in the mesh array for more information
        for (int m = 0; m < cJSON_GetArraySize(mesh_array); m++)
        {
            cJSON* mesh_entry = cJSON_GetArrayItem(mesh_array, m);
            char* mesh_data_name = cJSON_GetObjectItem(mesh_entry, "name")->valuestring;
            if (STRMATCH(mesh_data_name, node_data_name))
            {
                char* mesh_identifier = NULL;
                cJSON* primitive_array = cJSON_GetObjectItem(mesh_entry, "primitives");
                for (int p = 0; p < cJSON_GetArraySize(primitive_array); p++)
                {
                    cJSON* primative_entry = cJSON_GetArrayItem(primitive_array, p);
                    if (cJSON_HasObjectItem(primative_entry, "material"))
                    {
                        int prim_mat_index = cJSON_GetObjectItem(primative_entry, "material")->valueint;
                        cJSON* material_entry = cJSON_GetArrayItem(material_array, prim_mat_index);
                        char* material_data_name = cJSON_GetObjectItem(material_entry, "name")->valuestring;
                        mesh_identifier = TextFormat("%s-%s", node_data_name, material_data_name);
                    }
                    else
                    {
                        // Not a material mesh. Only use the mesh's ID as an indentifier
                        mesh_identifier = node_data_name;
                    }

                    MALLOC(MeshInfo, mesh_inf, NULL);
                    CHAR_STR_COPY(mesh_inf->mesh_name, mesh_identifier, NULL);
                    mesh_inf->mesh_index = mesh_load_index++;
                    hashmap_set(asset->mesh_data, mesh_inf);
                    printf(" ->%s\n", mesh_inf->mesh_name);
                }
                break;
            }
        }
    }
    cJSON_Delete(model_json);
    // Load animation data too
    asset->anm = LoadModelAnimations(path, &asset->anm_count);
    // This uses MEMSET, ensure all data is assigned before hashmapping!
    hashmap_set(loaded_assets, asset);
    if (!IsModelValid(*asset->mdl))
        printf("ASSET: Unable to load model: %s\n", path);
    else
        if (asset->anm != NULL && !IsModelAnimationValid(*asset->mdl, *asset->anm))
            printf("ASSET: Unable to load animations: %s\n", path);
        printf("ASSET: loaded model: %s\n", asset->filepath);
    return asset;
}

Asset* LoadAsset_Sound(char* path, int is_core)
{
    RETURN_EXISTING_ASSET(path, is_core);
    MALLOC_ASSET(asset, path, is_core);
    MALLOC_SET(Sound, asset->snd, FALSE);
    *asset->snd = LoadSound(path);
    // This uses MEMSET, ensure all data is assigned before hashmapping!
    hashmap_set(loaded_assets, asset);
    if (!IsSoundValid(*asset->snd))
        printf("ASSET: Unable to load sound: %s\n", path);
    else
        printf("ASSET: loaded sound: %s\n", asset->filepath);
    return asset;
}

Asset* LoadAsset_Music(char* path, int is_core)
{
    RETURN_EXISTING_ASSET(path, is_core);
    MALLOC_ASSET(asset, path, is_core);
    MALLOC_SET(Music, asset->mus, FALSE);
    *asset->mus = LoadMusicStream(path);
    // This uses MEMSET, ensure all data is assigned before hashmapping!
    hashmap_set(loaded_assets, asset);
    if (!IsMusicValid(*asset->mus))
        printf("ASSET: Unable to load music: %s\n", path);
    else
        printf("ASSET: loaded music: %s\n", asset->filepath);
    return asset;
}

Asset* LoadAsset_Material(char* path, int is_core)
{
    RETURN_EXISTING_ASSET(path, is_core);
    MALLOC_ASSET(asset, path, is_core);
    MALLOC_SET(Material, asset->mat, FALSE);
    *asset->mat = LoadMaterial(asset, path, is_core);
    // This uses MEMSET, ensure all data is assigned before hashmapping!
    hashmap_set(loaded_assets, asset);
    if (!IsMaterialValid(*asset->mat))
        printf("ASSET: Unable to load material: %s\n", path);
    else
        printf("ASSET: loaded material: %s\n", asset->filepath);
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
Material* AssetGet_Material(char* path) ASSET_FALLBACK(path, ASSET_MATERIALS"/Error/no_material.mat", mat);
