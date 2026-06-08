#include "animation.h"
#include <string.h>

ModelAnimation* GetAnimation(Asset* asset, char* name)
{
    if (asset->mdl == NULL || asset->anm == NULL)
        return NULL;
    for (int i = 0; i < asset->anm_count; i++)
    {
        if (strcmp(asset->anm[i].name, name) == 0)
            return &asset->anm[i];
    }
    return NULL;
}