#ifndef SCENE_ACTOR_HEADER
#define SCENE_ACTOR_HEADER

#include "scene_entry.h"

void LoadScene(SceneID id, EntranceID entrance);
void ReloadScene();
void UnloadScene();

void scene_actor_init(struct Actor* scene);
inline void SCENE_LIBRARY(struct Actor* scene, SceneID scene_id);

// All game scenes
void scene_debugscene_init(struct Actor* scene);
void scene_titlescene_init(struct Actor* scene);
void scene_testscene_init(struct Actor* scene);

#endif