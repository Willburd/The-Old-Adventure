#ifndef __MODELMATS_HEADER__
#define __MODELMATS_HEADER__

#include "raylib.h"

#define MATFLAG_BOTH_FACES (1 << 0)

Material LoadMaterial(Asset* asset, char* path, int is_core_asset);

// Set's material bitflags
void MaterialFlagSet(Material* mat, int matflag, int enable);
// Reads material bitflags
int MaterialFlagGet(Material* mat, int matflag);

#endif