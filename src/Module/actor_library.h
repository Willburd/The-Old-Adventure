#ifndef __ACTOR_LIBRARY_HEADER__
#define __ACTOR_LIBRARY_HEADER__

#include "actor.h"
#include "tools.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Actor library. Contains all actor polymorphs and where their init function pointers are.
// This only handles the func_init setup and call. Those functions set the rest of their pointers.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
	act_error,
	// debugging
	act_debug,
	// Gameengine
	act_scene,
	act_camera,
	// TODO - Your entities here
	LAST_ACTOR

} ActorTypes;

// debugging
ACTOR_INIT(debug);
// Gameengine
ACTOR_INIT(scene);
ACTOR_INIT(camera);
// TODO - Your entities here

#define ACTOR_STRING_CASE(str) if (STRMATCH(string_id, #str)) return act_## str
inline ActorTypes ACTOR_FROM_STRING(char* string_id)
{
	// debugging
	ACTOR_STRING_CASE(debug);
	// Gameengine
	ACTOR_STRING_CASE(scene);
	ACTOR_STRING_CASE(camera);
	// TODO - Your entities here
	return act_error;
}
#undef ACTOR_STRING_CASE

#define MAKE_ACTOR_INIT(x) case act_## x:{actor->actor_type_name = #x ;actor->func_init = actor_## x ##_init;if(actor->func_init != NULL) actor->func_init(actor);return;}break;
inline void ACTOR_LIBRARY(struct Actor* actor, ActorTypes actor_type)
{
	switch (actor_type)
	{
		// debugging
		MAKE_ACTOR_INIT(debug);
		// Gameengine
		MAKE_ACTOR_INIT(scene);
		MAKE_ACTOR_INIT(camera);
		// TODO - Your entities here
	}
}
#undef MAKE_ACTOR_INIT

#endif