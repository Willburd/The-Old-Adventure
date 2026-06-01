#ifndef TOOLS_HEADER
#define TOOLS_HEADER

#include "raylib.h"
#include "return_codes.h"

#define MALLOC(t,n) t* n = (t*)malloc(sizeof(t));if(n == NULL){printf("Failed malloc");exit(ERR_NOALLOC);};
#define CHAR_STR_COPY(s) (char*)malloc(strlen(s) + 1)

#endif