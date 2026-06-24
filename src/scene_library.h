#ifndef __SCENE_LIBRARY_HEADER__
#define __SCENE_LIBRARY_HEADER__

#include <stdio.h>
#include "actor.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Scene library. Contains all scene polymorphs and where their init function pointers are.
// This only handles the func_init setup and call. Those functions set the rest of their pointers.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
	// Setup
	scene_boot,
	// Debug
	scene_debug,
	scene_title,
	scene_test,
	scene_fieldtest,
	// Adventure
	LAST_SCENE

} SceneID;

SCENE_INIT(debug);
SCENE_INIT(boot);
SCENE_INIT(title);
SCENE_INIT(test);
SCENE_INIT(fieldtest);

inline char* scene_name(SceneID scene_id)
{
	char* scene_names[LAST_SCENE] = {
		// Setup
		"Boot",
		// Debug
		"Debug",
		"Title",
		"Test",
		"FieldTest"
		// Adventure
	};
	return scene_names[scene_id];
}

#define MAKE_SCENE_INIT(x,y) if(scene_id == x){scene->func_init = y;scene->func_init(scene);}
inline void SCENE_LIBRARY(struct Actor* scene, SceneID scene_id)
{
	printf("SCENE CHANGE: %s \n", scene_name(scene_id));
	// Setup
	MAKE_SCENE_INIT(scene_boot, scene_boot_init);
	// Debug
	MAKE_SCENE_INIT(scene_debug, scene_debug_init);
	MAKE_SCENE_INIT(scene_title, scene_title_init);
	MAKE_SCENE_INIT(scene_test, scene_test_init);
	MAKE_SCENE_INIT(scene_fieldtest, scene_fieldtest_init);
	// Adventure
}

#endif