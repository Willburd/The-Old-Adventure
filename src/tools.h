#ifndef TOOLS_HEADER
#define TOOLS_HEADER

#include "raylib.h"
#include "return_codes.h"

#define MALLOC(t,name,err) t* name = (t*)malloc(sizeof(t));if(name == NULL){printf("Failed malloc");exit(ERR_NOALLOC);return err;};
#define MALLOC_SET(t,name,err) name = (t*)malloc(sizeof(t));if(name == NULL){printf("Failed malloc");exit(ERR_NOALLOC);return err;};
#define MALLOC_SIZE(t,size,name,fill,err) t* name = (t*)malloc(size);if(name == NULL){printf("Failed malloc");exit(ERR_NOALLOC);return err;};for(int _mi = 0;_mi < size; ++_mi) { name[_mi] = fill;};
#define CHAR_STR_COPY(dest,src,err) dest = (char*)malloc(strlen(src)+1);if(dest == NULL){printf("Failed malloc");exit(ERR_NOALLOC);return err;}memcpy(dest, src, strlen(src));dest[strlen(src)]=0;

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#endif