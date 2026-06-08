#ifndef ACTOR_LIBRARY_HEADER
#define ACTOR_LIBRARY_HEADER

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
	act_animtest,
	// Gameengine
	act_scene,
	act_entrance,
	act_camera,
	act_skybox,
	// Adventure
	act_player,
	LAST_ACTOR

} ActorTypes;

void actor_debug_init(struct Actor* actor);
void actor_test_init(struct Actor* actor);
void actor_animationtest_init(struct Actor* actor);
void actor_scene_init(struct Actor* scene);
void actor_entrance_init(struct Actor* actor);
void actor_skybox_init(struct Actor* actor);
void actor_player_init(struct Actor* player);
void actor_camera_init(struct Actor* player);

inline char* actor_name(ActorTypes actor_id)
{
	char* actor_names[LAST_ACTOR] = {
		"Error",
		// debugging
		"Debug",
		"Test",
		"Animtest",
		// Gameengine
		"Scene",
		"Entrance",
		"Camera",
		"Skybox",
		// Adventure
		"Player"
	};
	return actor_names[actor_id];
}

#define MAKE_ACTOR_INIT(x,y) if(actor_type == x){actor->func_init = y; if(actor->func_init != NULL) actor->func_init(actor);}
inline void ACTOR_LIBRARY(struct Actor* actor, ActorTypes actor_type)
{
	MAKE_ACTOR_INIT(act_error, NULL);
	// debugging
	MAKE_ACTOR_INIT(act_debug, actor_debug_init);
	MAKE_ACTOR_INIT(act_test, actor_test_init);
	MAKE_ACTOR_INIT(act_animtest, actor_animationtest_init);
	// Gameengine
	MAKE_ACTOR_INIT(act_scene, actor_scene_init);
	MAKE_ACTOR_INIT(act_entrance, actor_entrance_init);
	MAKE_ACTOR_INIT(act_camera, actor_camera_init);
	MAKE_ACTOR_INIT(act_skybox, actor_skybox_init);
	// Adventure
	MAKE_ACTOR_INIT(act_player, actor_player_init);
}

#endif