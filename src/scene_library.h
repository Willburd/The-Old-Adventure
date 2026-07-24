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
	scene_Sboot,
	// Debug
	scene_Sdebug,
	// Adventure
	scene_Stitle,
	scene_Stest,
	scene_Sfieldtest,
	LAST_SCENE
} SceneID;
char* all_scene_names[500];

SCENE_INIT(Sdebug);
SCENE_INIT(Sboot);
SCENE_INIT(Stitle);
SCENE_INIT(Stest);
SCENE_INIT(Sfieldtest);

#define SCENE_STRING_CASE(str) if (STRMATCH(string_id, #str)) return scene_S## str
inline SceneID SCENE_FROM_STRING(char* string_id)
{
	// Setup
	SCENE_STRING_CASE(boot);
	// Debug
	SCENE_STRING_CASE(debug);
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
	PREDEF_SCENE(Sboot);
	// Debug
	PREDEF_SCENE(Sdebug);
	// Adventure
	PREDEF_SCENE(Stitle);
	PREDEF_SCENE(Stest);
	PREDEF_SCENE(Sfieldtest);
}
#undef PREDEF_SCENE

#define MAKE_SCENE_INIT(x) if(scene_id == scene_## x ){scene->actor_type_name = #x ;scene->func_init = scene_## x ##_init;scene->func_init(scene);}
inline void SCENE_LIBRARY(struct Actor* scene, SceneID scene_id)
{
	printf("SCENE CHANGE: %s \n", scene->actor_type_name);
	// Setup
	MAKE_SCENE_INIT(Sboot);
	// Debug
	MAKE_SCENE_INIT(Sdebug);
	MAKE_SCENE_INIT(Stitle);
	MAKE_SCENE_INIT(Stest);
	// Adventure
	MAKE_SCENE_INIT(Sfieldtest);
}
#undef MAKE_SCENE_INIT

#endif