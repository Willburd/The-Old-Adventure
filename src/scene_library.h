#ifndef SCENE_LIBRARY_HEADER
#define SCENE_LIBRARY_HEADER

#include "actor.h"

typedef enum
{
	debugscene,
	bootscene,
	titlescene,
	testscene,
	LAST_SCENE

} SceneID;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Scene library. Contains all scene polymorphs and where their init function pointers are.
// This only handles the func_init setup and call. Those functions set the rest of their pointers.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void scene_debugscene_init(struct Actor* scene);
void scene_titlescene_init(struct Actor* scene);
void scene_testscene_init(struct Actor* scene);

#define MAKE_SCENE_INIT(x,y) if(scene_id == x){scene->func_init = y;scene->func_init(scene);}
inline void SCENE_LIBRARY(struct Actor* scene, SceneID scene_id)
{
	MAKE_SCENE_INIT(debugscene, scene_debugscene_init);
	MAKE_SCENE_INIT(titlescene, scene_titlescene_init);
	MAKE_SCENE_INIT(testscene, scene_testscene_init);
}

#endif