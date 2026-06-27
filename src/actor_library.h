#ifndef __ACTOR_LIBRARY_HEADER__
#define __ACTOR_LIBRARY_HEADER__

#include "actor.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Actor library. Contains all actor polymorphs and where their init function pointers are.
// This only handles the func_init setup and call. Those functions set the rest of their pointers.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
	act_error,
	// debugging
	act_debug,
	act_test,
	act_animationtest,
	// Gameengine
	act_scene,
	act_entrance,
	act_trigger_exit,
	act_fadein,
	act_camera,
	act_skybox,
	act_textbox,
	// Adventure
	act_player,
	act_pause_box,
	act_signpost,
	act_fire,
	act_woodtorch,
	LAST_ACTOR

} ActorTypes;

ACTOR_INIT(debug);
ACTOR_INIT(test);
ACTOR_INIT(animationtest);
ACTOR_INIT(scene);
ACTOR_INIT(entrance);
ACTOR_INIT(trigger_exit);
ACTOR_INIT(fadein);
ACTOR_INIT(camera);
ACTOR_INIT(skybox);
ACTOR_INIT(textbox);
ACTOR_INIT(player);
ACTOR_INIT(pause_box);
ACTOR_INIT(signpost);
ACTOR_INIT(fire);
ACTOR_INIT(woodtorch);

#define MAKE_ACTOR_INIT(x) if(actor_type == act_## x){actor->actor_type_name = #x ;actor->func_init = actor_## x ##_init;if(actor->func_init != NULL) actor->func_init(actor);return;}
inline void ACTOR_LIBRARY(struct Actor* actor, ActorTypes actor_type)
{
	// debugging
	MAKE_ACTOR_INIT(debug);
	MAKE_ACTOR_INIT(test);
	MAKE_ACTOR_INIT(animationtest);
	// Gameengine
	MAKE_ACTOR_INIT(scene);
	MAKE_ACTOR_INIT(entrance);
	MAKE_ACTOR_INIT(trigger_exit);
	MAKE_ACTOR_INIT(fadein);
	MAKE_ACTOR_INIT(camera);
	MAKE_ACTOR_INIT(skybox);
	MAKE_ACTOR_INIT(textbox);
	// Adventure
	MAKE_ACTOR_INIT(player);
	MAKE_ACTOR_INIT(pause_box);
	MAKE_ACTOR_INIT(signpost);
	MAKE_ACTOR_INIT(fire);
	MAKE_ACTOR_INIT(woodtorch);
}
#undef MAKE_ACTOR_INIT

#endif