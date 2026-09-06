#ifndef __ACTOR_PLATFORM_HEADER__
#define __ACTOR_PLATFORM_HEADER__

void InitPlatformData(struct Actor* actor, float speed);
void HandlePlatformMove(struct Actor* actor);
void ApplyPlatformRotation(struct Actor* actor, struct Actor* platform, int influence_rotation);

#endif