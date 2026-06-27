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
	scene_Stitle,
	scene_Stest,
	scene_Sfieldtest,
	// Adventure
	LAST_SCENE
} SceneID;
char* all_scene_names[500];

SCENE_INIT(Sdebug);
SCENE_INIT(Sboot);
SCENE_INIT(Stitle);
SCENE_INIT(Stest);
SCENE_INIT(Sfieldtest);

#define PREDEF_SCENE(x) all_scene_names[scene_## x] = #x;
// Setup scene debug names
inline void SceneNamePredef()
{
	// Setup
	PREDEF_SCENE(Sdebug);
	// Debug
	PREDEF_SCENE(Sboot);
	PREDEF_SCENE(Stitle);
	PREDEF_SCENE(Stest);
	// Adventure
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