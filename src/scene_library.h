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
	// TODO - Your scenes here
	LAST_SCENE
} SceneID;
char* all_scene_names[500];

// Setup
SCENE_INIT(boot);
// Debug
SCENE_INIT(debugmenu);
// TODO - Your scenes here

#define SCENE_STRING_CASE(str) if (STRMATCH(string_id, #str)) return scene_## str
inline SceneID SCENE_FROM_STRING(char* string_id)
{
	// Setup
	SCENE_STRING_CASE(boot);
	// Debug
	SCENE_STRING_CASE(debugmenu);
	// TODO - Your scenes here
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
	// TODO - Your scenes here
}
#undef PREDEF_SCENE

#define MAKE_SCENE_INIT(x) if(scene_id == scene_## x ){scene->actor_type_name = #x ;scene->func_init = scene_## x ##_init;scene->func_init(scene);}
inline void SCENE_LIBRARY(struct Actor* scene, SceneID scene_id)
{
	// Setup
	MAKE_SCENE_INIT(boot);
	// Debug
	MAKE_SCENE_INIT(debugmenu);
	// TODO - Your scenes here


	printf("********************************************************************************\n");
	printf("SCENE CHANGE ==> %s \n", scene->actor_type_name);
	printf("..............................................................................\n\n");
}
#undef MAKE_SCENE_INIT

#endif