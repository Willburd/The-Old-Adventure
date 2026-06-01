#ifndef TOOLS_HEADER
#define TOOLS_HEADER

#include "raylib.h"
#include "return_codes.h"

#define MALLOC(t,n) t* n = (t*)malloc(sizeof(t));if(n == NULL){printf("Failed malloc");exit(ERR_NOALLOC);};
#define MALLOC_SIZE(t,size,name,fill) t* name = (t*)malloc(size);if(name == NULL){printf("Failed malloc");exit(ERR_NOALLOC);};for(int _mi = 0;_mi < size; ++_mi) { name[_mi] = fill;};
#define CHAR_STR_COPY(s) (char*)malloc(strlen(s) + 1)

#endif