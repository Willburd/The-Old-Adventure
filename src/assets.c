#include <stdlib.h>
#include <string.h>
#include "assets.h"
#include "animation.h"
#include "materials.h"
#include "models.h"
#include "tools.h"
#include "rlgl.h"

static struct hashmap* loaded_assets;


static int asset_compare(const void* a, const void* b, void* udata) {
    const Asset* ua = a;
    const Asset* ub = b;
    return strcmp(ua->filepath, ub->filepath);
}

static uint64_t asset_hash(const void* item, uint64_t seed0, uint64_t seed1) {
    const Asset* asset = item;
    return hashmap_sip(asset->filepath, strlen(asset->filepath), seed0, seed1);
}

static void asset_free(void* item) {
    Asset* asset = item;
    if (asset->tex != NULL && IsTextureValid(*asset->tex))
    {
        printf("UNLOAD TEXTURE------------------------\n");
        UnloadTexture(*asset->tex);
        RELEASE(asset->tex);
        printf("ASSET: texture unloaded %s\n", asset->filepath);
        printf("--------------------------------------\n");
    }
    if (asset->mdl != NULL && IsModelValid(*asset->mdl))
    {
        printf("UNLOAD MODEL--------------------------\n");
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
        printf("--------------------------------------\n");
    }
    if (asset->snd != NULL && IsSoundValid(*asset->snd))
    {
        printf("UNLOAD SOUND--------------------------\n");
        UnloadSound(*asset->snd);
        RELEASE(asset->snd);
        printf("ASSET: sound unloaded %s\n", asset->filepath);
        printf("--------------------------------------\n");
    }
    if (asset->mus != NULL && IsMusicValid(*asset->mus))
    {
        printf("UNLOAD MUSIC--------------------------\n");
        UnloadMusicStream(*asset->mus);
        RELEASE(asset->mus);
        printf("ASSET: music unloaded %s\n", asset->filepath);
        printf("--------------------------------------\n");
    }
    if (asset->mat != NULL && IsMaterialValid(*asset->mat))
    {
        printf("Unload Matrial------------------------\n");
        // Unload material's local textures

        // Unload map
        UnloadMaterial(*asset->mat);
        RELEASE(asset->mat);
        printf("ASSET: material unloaded %s\n", asset->filepath);
        printf("--------------------------------------\n");
    }
    RELEASE(asset->filepath); // malloc char* string
    free(asset->resource_ptr); // Release original allocation
}

void AssetHashmapCreate()
{
    loaded_assets = hashmap_new(sizeof(Asset), ASSET_LIMIT, 0, 0, asset_hash, asset_compare, asset_free, NULL);
}

void AssetHashmapDestroy()
{
    UnloadAllAssets(TRUE);
    hashmap_free(loaded_assets);
}

void AssetHashmapClear()
{
    UnloadAllAssets(TRUE);
    hashmap_clear(loaded_assets, FALSE);
}

void UnloadAllAssets(int including_core)
{
    printf("==============================================================================\n");
    if(!including_core)
        printf("                             UNLOADING ASSETS                                 \n");
    else
        printf("                          UNLOADING CORE ASSETS                               \n");
    printf("==============================================================================\n");

    size_t iter = 0;
    void* item;
    // Scan the hashmap for assets that are not core assets
    while (hashmap_iter(loaded_assets, &iter, &item)) {
        const Asset* search_asset = item;
        if (!including_core && search_asset->core_asset)
            continue;
        Asset* found_asset = hashmap_delete(loaded_assets, search_asset);
        if(found_asset != NULL)
            asset_free(found_asset);
    }
    if (including_core)
    {
        UnloadFont(default_font);
    }

    printf("==============================================================================\n");
    printf("                             UNLOADING FINISHED                               \n");
    printf("==============================================================================\n");
}

/// This uses MEMSET, ensure all data is assigned before pushing to the hashmap!
static void AssetPush(Asset* asset, int is_core, void* ext_data)
{
    char* path = asset->filepath;
    Asset* replaced = hashmap_set(loaded_assets, asset);
    if (hashmap_oom(loaded_assets))
    {
        printf("ASSET: ERROR, OUT OF MEMORY ON LOAD: %s\n", path);
        return;
    }
    if (replaced != NULL)
    {
        printf("ASSET: ERROR, DUPLICATED ASSET LOADED: %s\n", path);
        return;
    }
    // Check for hash collisions
    Asset* get_collision = AssetGetPackage(path);
    if (get_collision->resource_ptr != asset->resource_ptr) // If we don't get the same asset back, then it's wonked
    {
        printf("ASSET: ERROR, HASH COLLISION: %s vs %s\n", path, get_collision->filepath);
        return;
    }
    // Texture loading
    if (asset->tex != NULL)
    {
        if (!IsTextureValid(*asset->tex))
        {
            printf("ASSET: Unable to load texture: %s\n", path);
            return;
        }
        // Is valid
        if (ext_data != NULL)
            printf("ASSET: loaded material-texture: %s\n", path);
        else
            printf("ASSET: loaded texture: %s\n", path);
    }
    // Model loading
    if (asset->mdl != NULL)
    {
        if (!IsModelValid(*asset->mdl))
        {
            printf("ASSET: Unable to load model: %s\n", path);
            return;
        }
        if (asset->anm != NULL && !IsModelAnimationValid(*asset->mdl, *asset->anm))
        {
            printf("ASSET: Unable to load animations: %s\n", path);
            return;
        }
        // Is valid
        if(asset->anm != NULL)
            printf("ASSET: loaded model(has animations): %s\n", path);
        else
            printf("ASSET: loaded model: %s\n", path);
    }
    // Sound loading
    if (asset->snd != NULL)
    {
        if (!IsSoundValid(*asset->snd))
        {
            printf("ASSET: Unable to load sound: %s\n", path);
            return;
        }
        // Is valid
        printf("ASSET: loaded sound: %s\n", path);
    }
    // Music loading
    if (asset->mus != NULL)
    {
        if (!IsMusicValid(*asset->mus))
        {
            printf("ASSET: Unable to load music: %s\n", path);
            return;
        }
        // Is valid
        printf("ASSET: loaded music: %s\n", path);
    }
    // Material loading
    if (asset->mat != NULL)
    {
        if (!IsMaterialValid(*asset->mat))
        {
            printf("ASSET: Unable to load material: %s\n", path);
            return;
        }
        // Is valid
        printf("ASSET: loaded material: %s\n", asset->filepath);
    }
}


#define RETURN_EXISTING_ASSET(pth, s_core) Asset* check = AssetGetPackage(pth); if (check){if(s_core){check->core_asset=s_core;};printf("ASSET NOTICE: already loaded: %s\n", path);return check;}
#define RETURN_IF_NO_FILE(pth) if (!FileExists(pth)) {printf("!!!!! INVALID PATH: %s !!!!! \n", pth);printf("--------------------------------------\n");return NULL;}

Asset* LoadAsset_Texture(char* path, int is_core, char* mat_link)
{
    // Only valid for standalone textures, matlinked textures will not collide,
    // but the RAW path may exist for something else... Ignore it, we don't use it!
    // If it SOMEHOW does, AssetPush()'s final error checking will catch it.
    if (mat_link == NULL) 
    {
        RETURN_EXISTING_ASSET(path, is_core);
    }
    // Load texture
    printf("NEW TEXTURE---------------------------\n");
    RETURN_IF_NO_FILE(path);
    char* tex_name = mat_link != NULL ? TextFormat("%s[%s]", mat_link, path) : path;
    MALLOC_ASSET(asset, tex_name, is_core);
    MALLOC_SET(Texture2D, asset->tex, FALSE);
    *asset->tex = LoadTexture(path);
    AssetPush(asset, is_core, mat_link);
    printf("--------------------------------------\n");
    return asset;
}

Asset* LoadAsset_Model(char* path, int is_core)
{
    RETURN_EXISTING_ASSET(path, is_core);
    // Load model
    printf("NEW MODEL-----------------------------\n");
    RETURN_IF_NO_FILE(path);
    MALLOC_ASSET(asset, path, is_core);
    MALLOC_SET(Model, asset->mdl, FALSE);
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
        if (!cJSON_GetObjectItem(node_entry, "mesh")) // It's probably a bone or skeleton node
            continue;
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
                    char* material_data_name = "?";
                    cJSON* primative_entry = cJSON_GetArrayItem(primitive_array, p);
                    if (cJSON_HasObjectItem(primative_entry, "material"))
                    {
                        int prim_mat_index = cJSON_GetObjectItem(primative_entry, "material")->valueint;
                        cJSON* material_entry = cJSON_GetArrayItem(material_array, prim_mat_index);
                        material_data_name = cJSON_GetObjectItem(material_entry, "name")->valuestring;
                        mesh_identifier = TextFormat("%s-%s", node_data_name, material_data_name);
                    }
                    else
                    {
                        // Not a material mesh. Only use the mesh's ID as an indentifier
                        mesh_identifier = node_data_name;
                    }

                    MALLOC(MeshInfo, mesh_inf, NULL);
                    mesh_inf->resource_ptr = mesh_inf;
                    CHAR_STR_COPY(mesh_inf->mesh_name, mesh_identifier, NULL);
                    mesh_inf->mesh_index = mesh_load_index++;
                    CHAR_STR_COPY(mesh_inf->mat_name, material_data_name, NULL);
                    // Get material data for future reference
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
    AssetPush(asset, is_core, NULL);
    printf("--------------------------------------\n");
    return asset;
}

Asset* LoadAsset_Sound(char* path, int is_core)
{
    RETURN_EXISTING_ASSET(path, is_core);
    // Load sound
    printf("NEW SOUND-----------------------------\n");
    RETURN_IF_NO_FILE(path);
    MALLOC_ASSET(asset, path, is_core);
    MALLOC_SET(Sound, asset->snd, FALSE);
    *asset->snd = LoadSound(path);
    AssetPush(asset, is_core, NULL);
    printf("--------------------------------------\n");
    return asset;
}

Asset* LoadAsset_Music(char* path, int is_core)
{
    RETURN_EXISTING_ASSET(path, is_core);
    // Load music
    printf("NEW MUSIC-----------------------------\n");
    RETURN_IF_NO_FILE(path);
    MALLOC_ASSET(asset, path, is_core);
    MALLOC_SET(Music, asset->mus, FALSE);
    *asset->mus = LoadMusicStream(path);
    AssetPush(asset, is_core, NULL);
    printf("--------------------------------------\n");
    return asset;
}

Asset* LoadAsset_Material(char* path, int is_core)
{
    RETURN_EXISTING_ASSET(path, is_core);
    // Load material
    printf("NEW MATERIAL--------------------------\n");
    RETURN_IF_NO_FILE(path);
    MALLOC_ASSET(asset, path, is_core);
    MALLOC_SET(Material, asset->mat, FALSE);
    *asset->mat = LoadMaterial(asset, path, is_core);
    AssetPush(asset, is_core, NULL);
    printf("--------------------------------------\n");
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
Texture2D* AssetGet_Texture(char* path) ASSET_FALLBACK(path,ASSET_TEXTURES"/Engine/no_texture.png", tex);
Model* AssetGet_Model(char* path) ASSET_FALLBACK(path, ASSET_MODELS"/Tools/unit_cube.glb", mdl);
Sound* AssetGet_Sound(char* path) ASSET_FALLBACK(path, ASSET_TEXTURES"/Engine/no_texture.png", snd);
Music* AssetGet_Music(char* path) ASSET_FALLBACK(path, ASSET_TEXTURES"/Engine/no_texture.png", mus);
Material* AssetGet_Material(char* path) ASSET_FALLBACK(path, ASSET_MATERIALS"/Engine/no_material.mat", mat);

void LoadMaterialArray(char* mat_list[], int length)
{
    for (int i = 0; i < length; i++)
    {
        if (mat_list[i] == NULL) // Skip
            continue;
        LoadAsset_Material(mat_list[i], FALSE);
    }
}

// Notice, following functions are a bit unsafe if int/float ever change length...

void SetMaterialFlag(Material* mat, int flag, int enable)
{

}

int GetMaterialFlag(Material* mat, int flag)
{

}

// End notice.
