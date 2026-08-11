#ifndef __ACTOR_HEADER__
#define __ACTOR_HEADER__

#include <stdint.h>
#include "raylib.h"
#include "raymath.h"
#include "animation.h"

#define ANIMATION_LAYER_MAX 24
#define DEFAULT_MAX_RENDER_RANGE 200.0f

#define ACTOR_FLAG_DOES_NOT_TICK 0
// Must match gamestate flags
#define ACTOR_FLAG_NONE 0
#define ACTOR_FLAG_TICKDURING_GAME (1 << 0) // Updates during gameplay
#define ACTOR_FLAG_TICKDURING_TEXTBOX (1 << 1) // Updates during textboxes
#define ACTOR_FLAG_TICKDURING_TRANSITION (1 << 2) // Updates during room enter/exit animations
#define ACTOR_FLAG_TICKDURING_CUTSCENE (1 << 3) // Updates during cutscenes
#define ACTOR_FLAG_TICKDURING_PAUSED (1 << 4) // Updates during pause screen
// RESERVED (1 << 5)
// RESERVED (1 << 6)
// RESERVED (1 << 7)
// End of reserved
#define ACTOR_FLAG_HAS_ANIMATIONS (1 << 8) // Has animations
#define ACTOR_FLAG_IS_INVISIBLE (1 << 9) // disable drawing
#define ACTOR_FLAG_INTERACTIVE (1 << 10) // player can trigger interacts with
#define ACTOR_FLAG_DISABLE_LIGHTS (1 << 11) // Skips actor's lightnodes being queued to the shader

#define ACTOR_HAS_NO_ROOM_INDEX -1

#define SCENE_FLAG_GROUP_TEMP 0
#define SCENE_FLAG_GROUP_PERM 1
#define SCENE_FLAG_GROUP_PUZZLE 2

struct Actor;
struct Actor {
	int index;
	uint64_t uuid;
	int actor_type;
	char* actor_type_name;
	int is_destroying;

	struct Actor* parent;
	int current_room_index;
	void* data;

	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	Vector3 velocity;

	Vector3 last_position;
	Quaternion last_rotation;
	Vector3 last_scale;
	Vector3 last_velocity;

	// Generic flags for actor defined behaviors.
	unsigned int actor_flags;
	// Maximum range an actor is drawn at.
	float draw_range;
	// Color used various actors and shaders for actor defined reasons.
	Vector4 blend_color;
	// Scene flags set when this actor triggers a actor defined event.
	uint64_t triggers_flags;
	// Sets which flag group is written to when SceneFlagToggle() is called.
	int flag_group_selector;

	// Number of animation layers present in this actor
	int animlayer_count;
	// Stored animation layers, processed each update tick before drawing.
	struct AnimationLayer* animation_layers[ANIMATION_LAYER_MAX];

	// Actor setup and function assignment
	void (*func_init)(struct Actor* actor);
	// Load assets when actor is created
	void (*func_preloadassets)(struct Actor* actor);
	// Load settings from parsed json, must handle NULL file_data!
	void (*func_json_init)(struct Actor* actor, cJSON* file_data);
	// Actor cleanup during ACTOR_DESTROY()
	void (*func_destroy)(struct Actor* actor);
	// Actor update tick before primary update
	void (*func_preupdate)(struct Actor* actor);
	// Actor update tick
	void (*func_update)(struct Actor* actor);
	// Actor update tick after primary update
	void (*func_postupdate)(struct Actor* actor);
	// Add the lights of this object to the light queue
	void (*func_append_lights)(struct Actor* actor);
	// Actor draw world before primary draw
	void (*func_predrawworld)(struct Actor* actor, double tick_percent);
	// Actor draw world
	void (*func_drawworld)(struct Actor* actor, double tick_percent);
	// Actor draw world
	void (*func_transparentdrawworld)(struct Actor* actor, double tick_percent);
	// Actor draw world after primary draw
	void (*func_postdrawworld)(struct Actor* actor, double tick_percent);
	// Actor draw hud before primary draw
	void (*func_predrawhud)(struct Actor* actor, double tick_percent);
	// Actor draw hud
	void (*func_drawhud)(struct Actor* actor, double tick_percent);
	// Actor draw hud after primary draw
	void (*func_postdrawhud)(struct Actor* actor, double tick_percent);
	// Handle activating a room within a scene
	void (*func_activate_room)(struct Actor* actor, int room_index, int entrance);
	// Handle deactivating a room within a scene
	void (*func_deactivate_room)(struct Actor* actor, int room_index);
	// Handle animation end/loop actions
	void (*func_animation_ended)(struct Actor* actor, char* anim_name);
	// Handle interactions from the player
	void (*func_player_interact)(struct Actor* actor, struct Actor* player);
	// Handle if actor allows interactions with the player. If not set it will assume true.
	int (*func_can_interact)(struct Actor* actor, struct Actor* player);
	// Handle interaction text shown to the player when usable
	char* (*func_interaction_text)(struct Actor* actor, struct Actor* player);
	// Handle interactions from other actors
	void (*func_remote_interact)(struct Actor* actor, struct Actor* other_actor);
	// Handle collisions
	void (*func_touch)(struct Actor* actor, struct Actor* other);
};
#define ACTOR_CLEAR(x) \
x->uuid = 0; x->index = -1; x->actor_type = 0; x->actor_type_name = "?"; x->func_json_init = NULL; \
x->is_destroying = FALSE; \
x->parent = NULL; x->current_room_index = ACTOR_HAS_NO_ROOM_INDEX; \
x->actor_flags = 0; x->triggers_flags = 0; x->flag_group_selector = SCENE_FLAG_GROUP_TEMP; \
x->draw_range = DEFAULT_MAX_RENDER_RANGE; x->blend_color = (Vector4){ 255, 255, 255, 255}; \
x->animlayer_count = -1; \
x->func_init = NULL; x->func_preloadassets = NULL; \
x->func_destroy = NULL; \
x->func_preupdate = NULL; x->func_update = NULL; x->func_postupdate = NULL; \
x->func_append_lights = NULL; \
x->func_predrawworld = NULL; x->func_drawworld = NULL; x->func_transparentdrawworld = NULL; x->func_postdrawworld = NULL; \
x->func_predrawhud = NULL; x->func_drawhud = NULL; x->func_postdrawhud = NULL; \
x->func_activate_room = NULL; x->func_deactivate_room = NULL; \
x->func_animation_ended = NULL; \
x->func_player_interact = NULL;x->func_can_interact = NULL;x->func_interaction_text = NULL;x->func_remote_interact = NULL;x->func_touch = NULL; \
x->data = NULL;

#define ACTOR_POS_SNAP(x, pos) x->position = pos;x->last_position = pos;
#define ACTOR_ROT_SNAP(x, rot) x->rotation = rot;x->last_rotation = rot;
#define ACTOR_SCALE_SNAP(x, scl) x->scale = scl;x->last_scale = scl;
#define ACTOR_VEL_RESET(x, vel) x->velocity = vel;x->last_velocity = vel;
#define ACTOR_EXISTS(x) (x != NULL && !x->is_destroying)
#define ACTOR_HAS(x, func) (x->func != NULL)

#define ACTOR_POS_DELTA(x, delta) Vector3Lerp(x->last_position, x->position, (float)delta)
#define ACTOR_ROT_DELTA(x, delta) QuaternionLerp(x->last_rotation, x->rotation, (float)delta)
#define ACTOR_SCALE_DELTA(x, delta) Vector3Lerp(x->last_scale, x->scale, (float)delta)
#define ACTOR_VEL_DELTA(x, delta) Vector3Lerp(x->last_velocity, x->velocity, (float)delta)

#define ACTOR_PARENT(x) (x->parent)

#define ACTOR_LIMIT 8192

#define ACTOR_INTERACTION_RANGE 2.0f

/// Array of all actors
struct Actor* world_actors[ACTOR_LIMIT];	
/// Current highest actor array slot in memory. Shifts up and down as actors are reallocated to blank spots to close gaps from deletion.
int current_actor_cap;						
/// Total number of actors that exist in memory
int total_actors;							
/// Incrimented for every actor made. NOT used to index world_actors.
uint64_t current_unique_id;		

// Utility
// Find an actor by its uuid
struct Actor* FINDACTOR(uint64_t id);
// Find multiple actors by their uuids
void FINDACTORGROUP(const struct Actor* found_group[], int max_count, const uint64_t find_uuids[]);
// Get first actor of a specified type
struct Actor* FINDACTORTYPE(int actor_type);
// Get all actors of a specific type
void FINDALLACTORTYPE(const struct Actor* found_group[], int max_count, int actor_type);
// Find actor nearest to a specified point, also allows filtering out a specified actor during the search
struct Actor* FINDACTORNEAREST(Vector3 at_pos, struct Actor* ignored_actor);
// Find the actor furthest from a specified point, also allows filtering out a specified actor during the search
struct Actor* FINDACTORFURTHEST(Vector3 at_pos, struct Actor* ignored_actor);
// Find the nearest actor that allows interaction, also allows filtering out a specified actor during the search
struct Actor* FINDINTERACTIONNEAREST(Vector3 at_pos, struct Actor* ignored_actor);
// Number of actors of this type
int ACTORCOUNT(int actor_type);
// Get first child of parent
struct Actor* FINDCHILD(const struct Actor* parent);
// Get all children of parent
void FINDACTORCHILDREN(const struct Actor* found_group[], int max_count, const struct Actor* parent);
// Get number of children a parent has
int CHILDCOUNT(const struct Actor* parent);
// Get parent scene
struct Actor* GETSCENE(struct Actor* actor);
// Get matrix of model
Matrix GetMatrix(struct Actor* actor);
// Get transform of model
Transform GetTransform(struct Actor* actor);
// Check if we are in range of the camera before drawing
int OutOfRenderRange(struct Actor* actor);

// Automatically activates the current scene's flags based on the flags configured in the actor.
void SceneFlagTrigger(struct Actor* actor);
// Automatically clears the current scene's flags based on the flags configured in the actor.
void SceneFlagClear(struct Actor* actor);
// Automatically toggles the current scene's flags based on the flags configured in the actor.
void SceneFlagToggle(struct Actor* actor);

// definition boilerplate for actors
#define ACTOR_INIT(x) void actor_## x ##_init(struct Actor* actor)
#define ACTOR_PRELOADASSETS(x) static void actor_## x ##_preload_assets(struct Actor* actor)
#define ACTOR_JSON_INIT(x) static void actor_## x ##_json_init(struct Actor* actor, cJSON* file_data)
#define ACTOR_CLEANUP(x) static void actor_## x ##_cleanup(struct Actor* actor)

#define ACTOR_PREUPDATE(x) static void actor_## x ##_preupdate(struct Actor* actor)
#define ACTOR_UPDATE(x) static void actor_## x ##_update(struct Actor* actor)
#define ACTOR_POSTUPDATE(x) static void actor_## x ##_postupdate(struct Actor* actor)

#define ACTOR_LIGHTNODES(x) static void actor_## x ##_lights(struct Actor* actor)
#define ACTOR_ANIMATION_END(x) static void actor_## x ##_animation_ended(struct Actor* actor, char* animation)

#define ACTOR_PLAYER_INTERACT(x) static void actor_## x ##_player_interact(struct Actor* actor, struct Actor* player)
#define ACTOR_CAN_INTERACT(x) static int actor_## x ##_can_interact(struct Actor* actor, struct Actor* player)
#define ACTOR_INTERACT_TEXT(x) static char* actor_## x ##_interaction_text(struct Actor* actor, struct Actor* player)
#define ACTOR_REMOTE_INTERACT(x) static void actor_## x ##_remote_interact(struct Actor* actor, struct Actor* other)

#define ACTOR_PREDRAWWORLD(x) static void actor_## x ##_predrawworld(struct Actor* actor, double tick_percent)
#define ACTOR_DRAWWORLD(x) static void actor_## x ##_drawworld(struct Actor* actor, double tick_percent)
#define ACTOR_TRANSPARENTDRAWWORLD(x) static void actor_## x ##_transparentdrawworld(struct Actor* actor, double tick_percent)
#define ACTOR_POSTDRAWWORLD(x) static void actor_## x ##_postdrawworld(struct Actor* actor, double tick_percent)

#define ACTOR_PREDRAWHUD(x) static void actor_## x ##_predrawhud(struct Actor* actor, double tick_percent)
#define ACTOR_DRAWHUD(x) static void actor_## x ##_drawhud(struct Actor* actor, double tick_percent)
#define ACTOR_POSTDRAWHUD(x) static void actor_## x ##_postdrawhud(struct Actor* actor, double tick_percent)

// registration boilerplate for actors
#define ACTOR_REGISTER_PRELOADASSETS(x) actor->func_preloadassets = actor_## x ##_preload_assets
#define ACTOR_REGISTER_JSON_INIT(x) actor->func_json_init = actor_## x ##_json_init
#define ACTOR_REGISTER_CLEANUP(x) actor->func_destroy = actor_## x ##_cleanup

#define ACTOR_REGISTER_PREUPDATE(x) actor->func_preupdate = actor_## x ##_preupdate
#define ACTOR_REGISTER_UPDATE(x) actor->func_update = actor_## x ##_update
#define ACTOR_REGISTER_POSTUPDATE(x) actor->func_postupdate = actor_## x ##_postupdate

#define ACTOR_REGISTER_LIGHTNODES(x) actor->func_append_lights = actor_## x ##_lights
#define ACTOR_REGISTER_ANIMATION_END(x) actor->func_animation_ended = actor_## x ##_animation_ended

#define ACTOR_REGISTER_PLAYER_INTERACT(x) actor->func_player_interact = actor_## x ##_player_interact
#define ACTOR_REGISTER_CAN_INTERACT(x) actor->func_can_interact = actor_## x ##_can_interact
#define ACTOR_REGISTER_INTERACT_TEXT(x) actor->func_interaction_text = actor_## x ##_interaction_text
#define ACTOR_REGISTER_REMOTE_INTERACT(x) actor->func_remote_interact = actor_## x ##_remote_interact

#define ACTOR_REGISTER_PREDRAWWORLD(x) actor->func_predrawworld = actor_## x ##_predrawworld
#define ACTOR_REGISTER_DRAWWORLD(x) actor->func_drawworld = actor_## x ##_drawworld
#define ACTOR_REGISTER_TRANSPARENTDRAWWORLD(x) actor->func_transparentdrawworld = actor_## x ##_transparentdrawworld
#define ACTOR_REGISTER_POSTDRAWWORLD(x) actor->func_postdrawworld = actor_## x ##_postdrawworld

#define ACTOR_REGISTER_PREDRAWHUD(x) actor->func_predrawhud = actor_## x ##_predrawhud
#define ACTOR_REGISTER_DRAWHUD(x) actor->func_drawhud = actor_## x ##_drawhud
#define ACTOR_REGISTER_POSTDRAWHUD(x) actor->func_postdrawhud = actor_## x ##_postdrawhud

// definition boilerplate for scenes
#define SCENE_INIT(x) void scene_## x ##_init(struct Actor* scene)
#define SCENE_PRELOADASSETS(x) static void scene_## x ##_preload_assets(struct Actor* scene)
#define SCENE_ACTIVATE_ROOM(x) static void scene_## x ##_activate_room(struct Actor* scene, int room_index, int entrance)
#define SCENE_CLEANUP(x) static void scene_## x ##_cleanup(struct Actor* scene)

#define SCENE_UPDATE(x) static void scene_## x ##_update(struct Actor* scene)
#define SCENE_DRAWWORLD(x) static void scene_## x ##_drawworld(struct Actor* scene, double tick_percent)
#define SCENE_DRAWHUD(x) static void scene_## x ##_drawhud(struct Actor* scene, double tick_percent)
#define SCENE_LIGHTNODES(x) static void scene_## x ##_lights(struct Actor* scene)

// registration boilerplate for scenes
#define SCENE_REGISTER_PRELOADASSETS(x) scene->func_preloadassets = scene_## x ##_preload_assets
#define SCENE_REGISTER_ACTIVATE_ROOM(x) scene->func_activate_room = scene_## x ##_activate_room
#define SCENE_REGISTER_CLEANUP(x) scene->func_destroy = scene_## x ##_cleanup

#define SCENE_REGISTER_UPDATE(x) scene->func_update = scene_## x ##_update
#define SCENE_REGISTER_DRAWWORLD(x) scene->func_drawworld = scene_## x ##_drawworld
#define SCENE_REGISTER_TRANSPARENTDRAWWORLD(x) scene->func_transparentdrawworld = scene_## x ##_transparentdrawworld
#define SCENE_REGISTER_DRAWHUD(x) scene->func_drawhud = scene_## x ##_drawhud
#define SCENE_REGISTER_LIGHTNODES(x) scene->func_append_lights = scene_## x ##_lights


#endif