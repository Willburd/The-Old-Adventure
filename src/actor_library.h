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
	act_fadein,
	act_fadeout,
	act_camera,
	// Adventure
	act_test,
	act_animationtest,
	act_entrance,
	act_trigger_exit,
	act_skybox,
	act_textbox,
	act_player,
	act_pause_box,
	act_signpost,
	act_fire,
	act_woodtorch,
	act_tree,
	act_hole,
	act_crate,
	act_smallrock,
	act_roomfade,
	LAST_ACTOR
} ActorTypes;

// debugging
ACTOR_INIT(debug);
// Gameengine
ACTOR_INIT(scene);
ACTOR_INIT(fadein);
ACTOR_INIT(fadeout);
ACTOR_INIT(camera);
// Adventure
ACTOR_INIT(test);
ACTOR_INIT(animationtest);
ACTOR_INIT(entrance);
ACTOR_INIT(trigger_exit);
ACTOR_INIT(skybox);
ACTOR_INIT(textbox);
ACTOR_INIT(player);
ACTOR_INIT(pause_box);
ACTOR_INIT(signpost);
ACTOR_INIT(fire);
ACTOR_INIT(woodtorch);
ACTOR_INIT(tree);
ACTOR_INIT(hole);
ACTOR_INIT(crate);
ACTOR_INIT(smallrock);
ACTOR_INIT(roomfade);

#define ACTOR_STRING_CASE(str) if (STRMATCH(string_id, #str)) return act_## str
inline ActorTypes ACTOR_FROM_STRING(char* string_id)
{
	// debugging
	ACTOR_STRING_CASE(debug);
	// Gameengine
	ACTOR_STRING_CASE(scene);
	ACTOR_STRING_CASE(fadein);
	ACTOR_STRING_CASE(fadeout);
	ACTOR_STRING_CASE(camera);
	// Adventure
	ACTOR_STRING_CASE(test);
	ACTOR_STRING_CASE(animationtest);
	ACTOR_STRING_CASE(entrance);
	ACTOR_STRING_CASE(trigger_exit);
	ACTOR_STRING_CASE(skybox);
	ACTOR_STRING_CASE(textbox);
	ACTOR_STRING_CASE(player);
	ACTOR_STRING_CASE(pause_box);
	ACTOR_STRING_CASE(signpost);
	ACTOR_STRING_CASE(fire);
	ACTOR_STRING_CASE(woodtorch);
	ACTOR_STRING_CASE(tree);
	ACTOR_STRING_CASE(hole);
	ACTOR_STRING_CASE(crate);
	ACTOR_STRING_CASE(smallrock);
	ACTOR_STRING_CASE(roomfade);
	return act_error;
}
#undef ACTOR_STRING_CASE

#define MAKE_ACTOR_INIT(x) if(actor_type == act_## x){actor->actor_type_name = #x ;actor->func_init = actor_## x ##_init;if(actor->func_init != NULL) actor->func_init(actor);return;}
inline void ACTOR_LIBRARY(struct Actor* actor, ActorTypes actor_type)
{
	// debugging
	MAKE_ACTOR_INIT(debug);
	// Gameengine
	MAKE_ACTOR_INIT(scene);
	MAKE_ACTOR_INIT(fadein);
	MAKE_ACTOR_INIT(fadeout);
	MAKE_ACTOR_INIT(camera);
	// Adventure
	MAKE_ACTOR_INIT(test);
	MAKE_ACTOR_INIT(animationtest);
	MAKE_ACTOR_INIT(entrance);
	MAKE_ACTOR_INIT(trigger_exit);
	MAKE_ACTOR_INIT(skybox);
	MAKE_ACTOR_INIT(textbox);
	MAKE_ACTOR_INIT(player);
	MAKE_ACTOR_INIT(pause_box);
	MAKE_ACTOR_INIT(signpost);
	MAKE_ACTOR_INIT(fire);
	MAKE_ACTOR_INIT(woodtorch);
	MAKE_ACTOR_INIT(tree);
	MAKE_ACTOR_INIT(hole);
	MAKE_ACTOR_INIT(crate);
	MAKE_ACTOR_INIT(smallrock);
	MAKE_ACTOR_INIT(roomfade);
}
#undef MAKE_ACTOR_INIT

#endif