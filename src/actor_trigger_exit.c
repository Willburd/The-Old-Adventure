#include "tools.h"
#include "assets.h"
#include "actor_trigger_exit.h"
#include "actor_factory.h"
#include "scene_entry.h"

// private header
static void actor_trigger_exit_preupdate(struct Actor* actor);
static void actor_trigger_exit_lights(struct Actor* scene);
static void actor_trigger_exit_drawworld(struct Actor* actor, double tick_percent);
static void actor_exit_startleaving(struct Actor* exit);
static void actor_exit_finishleaving(struct Actor* exit);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Setup the player actor. Public function in the header
void actor_trigger_exit_init(struct Actor* actor)
{
	// Configure actor
	actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME;
	actor->func_preupdate = actor_trigger_exit_preupdate;
	actor->func_drawworld = actor_trigger_exit_drawworld;

	// Set data
	MALLOC_ACTOR_DATA(TriggerExitData, actor->data);
}

struct Actor* EXIT_TRIGGER_CREATE(int destination_scene, int destination_entrance, struct Actor* scene, Vector3 pos, float radius)
{
	struct Actor* exit = ACTOR_FACTORY(act_trigger_exit, scene, pos, QuaternionIdentity(), Vector3One(), Vector3Zero());
	TriggerExitData* exit_data = exit->data;
	exit_data->dest_scene = destination_scene;
	exit_data->dest_entrance = destination_entrance;
	exit_data->radius = radius;
	return exit;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void actor_trigger_exit_preupdate(struct Actor* exit)
{
	struct Actor* player = FINDACTORTYPE(act_player);
	if (!player)
		return;

	TriggerExitData* exit_data = exit->data;

	if (Vector3Distance(exit->position, player->position) < exit_data->radius)
	{
		actor_exit_startleaving(exit);
	}
}

static void actor_trigger_exit_drawworld(struct Actor* exit, double tick_percent)
{
	TriggerExitData* exit_data = exit->data;
	DrawSphereWires(exit->position, exit_data->radius, 10, 10, WHITE);
}

static void actor_exit_startleaving(struct Actor* exit)
{
	TriggerExitData* exit_data = exit->data;
	LoadScene(exit_data->dest_scene, exit_data->dest_entrance);
}

static void actor_exit_finishleaving(struct Actor* exit)
{
	TriggerExitData* exit_data = exit->data;
	LoadScene(exit_data->dest_scene, exit_data->dest_entrance);
}
