#ifndef SCENE_ACTOR_HEADER
#define SCENE_ACTOR_HEADER

#include "scene_entry.h"

void LoadScene(SceneID id, EntranceID entrance);
void ReloadScene();
void UnloadScene();

inline void SCENE_LIBRARY(struct Actor* scene, SceneID scene_id);

#endif