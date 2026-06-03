#ifndef TOOLS_HEADER
#define TOOLS_HEADER

#include "raylib.h"
#include "return_codes.h"
#include "stdlib.h"

#define MALLOC(t,name,err) t* name = (t*)malloc(sizeof(t));if(name == NULL){printf("Failed malloc");exit(ERR_NOALLOC);return err;};
#define MALLOC_SET(t,name,err) name = (t*)malloc(sizeof(t));if(name == NULL){printf("Failed malloc");exit(ERR_NOALLOC);return err;};
#define MALLOC_ACTOR_DATA(t,name) name = (t*)malloc(sizeof(t));if(name == NULL){printf("Failed malloc");exit(ERR_NOALLOC);};
#define MALLOC_SIZE(t,size,name,fill,err) t* name = (t*)malloc(size);if(name == NULL){printf("Failed malloc");exit(ERR_NOALLOC);return err;};for(int _mi = 0;_mi < size; ++_mi) { name[_mi] = fill;};
#define CHAR_STR_COPY(dest,src,err) dest = (char*)malloc(strlen(src)+1);if(dest == NULL){printf("Failed char string malloc");exit(ERR_NOALLOC);return err;}memcpy(dest, src, strlen(src));dest[strlen(src)]=0;

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

#define RAND_RANGE(l,h) ((l) + (rand() % ((h)-(l))))

#define STRMATCH(x,y) (strcmp(x,y)==0)

#define VEC3UP ((Vector3){0,1,0})
#define VEC3DOWN ((Vector3){0,-1,0})
#define VEC3FORWARD ((Vector3){0,0,-1})
#define VEC3BACKWARD ((Vector3){0,0,1})
#define VEC3RIGHT ((Vector3){1,0,0})
#define VEC3LEFT ((Vector3){-1,0,0})

#define RELEASE(x) free(x);x = NULL;

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#endif