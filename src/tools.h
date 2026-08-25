#ifndef __TOOLS_HEADER__
#define __TOOLS_HEADER__

#include "raylib.h"
#include "raymath.h"
#include "return_codes.h"
#include "stdlib.h"
#include "cJSON.h"

#define MALLOC(t,name,err) t* name = (t*)malloc(sizeof(t));if(name == NULL){printf("Failed malloc");exit(ERR_NOALLOC);return err;};
#define MALLOC_SET(t,name,err) name = (t*)malloc(sizeof(t));if(name == NULL){printf("Failed malloc");exit(ERR_NOALLOC);return err;};
#define MALLOC_ACTOR_DATA(t,name) name = (t*)malloc(sizeof(t));if(name == NULL){printf("Failed malloc");exit(ERR_NOALLOC);};
#define MALLOC_SIZE(t,size,name,fill,err) t* name = (t*)malloc(size);if(name == NULL){printf("Failed malloc");exit(ERR_NOALLOC);return err;};for(int _mi = 0;_mi < size; ++_mi) { name[_mi] = fill;};
#define MALLOC_SET_SIZE(t,size,name,fill,err) name = (t*)malloc(size);if(name == NULL){printf("Failed malloc");exit(ERR_NOALLOC);return err;};for(int _mi = 0;_mi < size; ++_mi) { name[_mi] = fill;};
#define CHAR_STR_COPY(dest,src,err) dest = (char*)malloc(strlen(src)+1);if(dest == NULL){printf("Failed char string malloc");exit(ERR_NOALLOC);return err;}memcpy(dest, src, strlen(src));dest[strlen(src)]=0;

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

#define RAND_RANGE(l,h) ((l) + (rand() % ((h)-(l))))
#define RAND_FRANGE(l,h) ((l) + ((float)fmod((float)rand()/1000.0f,((h)-(l)))))
#define SIGN(x) ((x > 0) - (x < 0))
#define CIRCLE_DEGREES 360.0f

#define STRMATCH(x,y) (strcmp(x,y)==0)
#define STRENDLINETERMINATE(x) x[strcspn(x, "\n")] = '\0'

#define CHECK_JSON_BOOL(d, x) (cJSON_GetObjectItem(d, x) && cJSON_GetObjectItem(d, x)->valueint > 0)

#define VEC3UP ((Vector3){0,1,0})
#define VEC3DOWN ((Vector3){0,-1,0})
#define VEC3FORWARD ((Vector3){0,0,-1})
#define VEC3BACKWARD ((Vector3){0,0,1})
#define VEC3RIGHT ((Vector3){1,0,0})
#define VEC3LEFT ((Vector3){-1,0,0})
#define VEC3DIRECTION(s,e) Vector3Normalize(Vector3Subtract((e), (s)))
#define VEC2DIRECTION(s,e) Vector2Normalize(Vector2Subtract((e), (s)))

#define RELEASE(x) free(x);x = NULL;

#define MATRIX_ASSEMBLE(transform) MatrixMultiply(MatrixMultiply(MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z), QuaternionToMatrix(transform.rotation)), MatrixTranslate(transform.translation.x, transform.translation.y, transform.translation.z))

#define DRAWCAPSULE(pos, hig, rad, col) DrawCapsule(Vector3Add(pos, Vector3Scale(VEC3UP, rad)), Vector3Add(pos, Vector3Scale(VEC3UP, rad + hig + rad)), rad, 7, 5, col)

/// Gets a fake-random value based on the position and a custom offset.
int GetFixedRandomInt(Vector3 pos, int offset);
float GetFixedRandomFloat(Vector3 pos, int offset);

Vector4 ColorToVector4(Color col);
Vector3 ColorToVector3(Color col);
Color Vector4ToColor(Vector4 vec);
Color Vector3ToColor(Vector3 vec, float alpha);
float Vector3GetTopDownAngle(Vector3 direction);
Vector3 Vector3FlatDirection(Vector3 start, Vector3 end);
Quaternion QuaternionLookAt(Vector3 start, Vector3 end, Vector3 up);
Quaternion QuaternionFlatLookAt(Vector3 start, Vector3 end, Vector3 up);
Quaternion QuaternionGetFlat(Quaternion source, Vector3 up);
void ApplyFriction(struct Actor* actor, float amount);
void ApplyFlatFriction(struct Actor* actor, float amount);
cJSON* ParseJsonFile(char* path);

#endif

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif