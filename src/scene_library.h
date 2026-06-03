#ifndef SCENE_LIBRARY_HEADER
#define SCENE_LIBRARY_HEADER

#include <stdio.h>
#include "actor.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Scene library. Contains all scene polymorphs and where their init function pointers are.
// This only handles the func_init setup and call. Those functions set the rest of their pointers.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
	scene_boot,
	scene_debug,
	scene_title,
	scene_test,
	LAST_SCENE

} SceneID;

void scene_debug_init(struct Actor* scene);
void scene_boot_init(struct Actor* scene);
void scene_title_init(struct Actor* scene);
void scene_test_init(struct Actor* scene);

inline char* scene_name(SceneID scene_id)
{
	char* scene_names[LAST_SCENE] = {
		"Boot",
		"Debug",
		"Title",
		"Test"
	};
	return scene_names[scene_id];
}

#define MAKE_SCENE_INIT(x,y) if(scene_id == x){scene->func_init = y;scene->func_init(scene);}
inline void SCENE_LIBRARY(struct Actor* scene, SceneID scene_id)
{
	printf("SCENE CHANGE: %s \n", scene_name(scene_id));
	MAKE_SCENE_INIT(scene_boot, scene_boot_init);
	MAKE_SCENE_INIT(scene_debug, scene_debug_init);
	MAKE_SCENE_INIT(scene_title, scene_title_init);
	MAKE_SCENE_INIT(scene_test, scene_test_init);
}

#endif