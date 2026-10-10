#ifndef __ACTOR_PLATFORM_HEADER__
#define __ACTOR_PLATFORM_HEADER__

#include "../cJSON/cJSON.h"

void InitPlatformData(struct Actor* actor, float speed, int is_moving, int is_reversed);
void InitPlatformJson(struct Actor* actor, cJSON* file_data);
void HandlePlatformMove(struct Actor* actor);
void ApplyPlatformRotation(struct Actor* actor, struct Actor* platform, int influence_rotation);
void HandlePlatformCleanup(struct Actor* actor);

#endif