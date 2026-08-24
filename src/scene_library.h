#ifndef __SCENE_LIBRARY_HEADER__
#define __SCENE_LIBRARY_HEADER__

#include <stdio.h>
#include "actor.h"
#include "tools.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Scene library. Contains all scene polymorphs and where their init function pointers are.
// This only handles the func_init setup and call. Those functions set the rest of their pointers.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
	// Setup
	scene_boot,
	// Debug
	scene_debugmenu,
	// Adventure
	scene_title,
	scene_test,
	scene_fieldtest,
	LAST_SCENE
} SceneID;
char* all_scene_names[500];

// Setup
SCENE_INIT(boot);
// Debug
SCENE_INIT(debugmenu);
// Adventure
SCENE_INIT(title);
SCENE_INIT(test);
SCENE_INIT(fieldtest);

#define SCENE_STRING_CASE(str) if (STRMATCH(string_id, #str)) return scene_## str
inline SceneID SCENE_FROM_STRING(char* string_id)
{
	// Setup
	SCENE_STRING_CASE(boot);
	// Debug
	SCENE_STRING_CASE(debugmenu);
	// Adventure
	SCENE_STRING_CASE(title);
	SCENE_STRING_CASE(test);
	SCENE_STRING_CASE(fieldtest);
	return LAST_SCENE;
}
#undef SCENE_STRING_CASE

#define PREDEF_SCENE(x) all_scene_names[scene_## x] = #x;
// Setup scene debug names
inline void SceneNamePredef()
{
	// Setup
	PREDEF_SCENE(boot);
	// Debug
	PREDEF_SCENE(debugmenu);
	// Adventure
	PREDEF_SCENE(title);
	PREDEF_SCENE(test);
	PREDEF_SCENE(fieldtest);
}
#undef PREDEF_SCENE

#define MAKE_SCENE_INIT(x) case scene_## x:{scene->actor_type_name = #x ;scene->func_init = scene_## x ##_init;scene->func_init(scene);}break;
inline void SCENE_LIBRARY(struct Actor* scene, SceneID scene_id)
{
	switch (scene_id)
	{
		// Setup
		MAKE_SCENE_INIT(boot);
		// Debug
		MAKE_SCENE_INIT(debugmenu);
		// Adventure
		MAKE_SCENE_INIT(title);
		MAKE_SCENE_INIT(test);
		MAKE_SCENE_INIT(fieldtest);
	}

	printf("********************************************************************************\n");
	printf("SCENE CHANGE ==> %s \n", scene->actor_type_name);
	printf("..............................................................................\n\n");
}
#undef MAKE_SCENE_INIT

#endif
