#include "assets.h"

int asset_compare(const void* a, const void* b, void* udata) {
    const Asset* ua = a;
    const Asset* ub = b;
    return strcmp(ua->filepath, ub->filepath);
}

__int64 asset_hash(const void* item, __int64 seed0, __int64 seed1) {
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
}


void LoadAsset_Texture(const char* path)
{
	Texture2D tex = LoadTexture(path);
	
}

void LoadAsset_Model(char* path)
{
	Model mdl = LoadModel(path);

}

void LoadAsset_Sound(const char* path)
{
	Sound snd = LoadSound(path);

}

void LoadAsset_Music(const char* path)
{
	Music mus = LoadMusicStream(path);

}