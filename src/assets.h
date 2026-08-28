#ifndef __ASSETS_HEADER__
#define __ASSETS_HEADER__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "raylib.h"
#include "hashmap.h"
#include "cJSON.h"

// TODO - Properly set something up for this
#ifdef _RELEASE
#define ASSET_PATH "./Assets"
#else
#define ASSET_PATH "../Assets"
#endif

#define ASSET_TEXTURES ASSET_PATH"/Textures"
#define ASSET_MODELS ASSET_PATH"/Models"
#define ASSET_MATERIALS ASSET_PATH"/Materials"
#define ASSET_SHADERS ASSET_PATH"/Shaders"
#define ASSET_TEXT ASSET_PATH"/Text"
#define ASSET_FONT ASSET_PATH"/Font"
#define ASSET_SCENE ASSET_PATH"/Scenes"

#define ASSET_LIMIT 2048

Font default_font;

enum GameLanguages
{
    lang_EN,
};
int current_game_language;

typedef struct {
    void* resource_ptr;
    int core_asset;
    char* filepath;
    Texture2D* tex;
    Model* mdl;
    struct hashmap* mesh_data;
    Sound* snd;
    Music* mus;
    Material* mat;
    int anm_count;
    ModelAnimation* anm;
} Asset;

#define MALLOC_ASSET(a, p, s_core) MALLOC(Asset, a, 0);a->core_asset=s_core;CHAR_STR_COPY(a->filepath, p, 0);a->tex=NULL;a->mdl=NULL;a->mesh_data=NULL;a->snd=NULL;a->mus=NULL;a->mat=NULL;a->anm=NULL;a->anm_count=0;a->resource_ptr=a;

// Remove all assets from the hashmap. Normally ignores core assets.
void AssetHashmapCreate();
void AssetHashmapDestroy();
void AssetHashmapClear();
void UnloadAllAssets(int including_core);

Asset* LoadAsset_Texture(char* path, int is_core, char* mat_link);
Asset* LoadAsset_Model(char* path, int is_core);
Asset* LoadAsset_Sound(char* path, int is_core);
Asset* LoadAsset_Music(char* path, int is_core);
Asset* LoadAsset_Material(char* path, int is_core);
int AssetExists(char* path);
Asset* AssetGetPackage(char* path);

Texture2D* AssetGet_Texture(char* path);
Model* AssetGet_Model(char* path);
Sound* AssetGet_Sound(char* path);
Music* AssetGet_Music(char* path);
Material* AssetGet_Material(char* path);

// Loads all material paths provided from an array
void LoadMaterialArray(char* mat_list[], int length);

// Set's material bitflags
void SetMaterialFlag(Material* mat, int flag, int enable);
// Reads material bitflags
int GetMaterialFlag(Material* mat, int flag);
#endif
