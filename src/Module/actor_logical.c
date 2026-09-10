#include "../tools.h"
#include "../actor_factory.h"
#include "json_properties.h"

// private header
typedef struct {
	char* target;
	// Number of times the element has fired
	int trigger_count;
	// Number of times the element needs to be triggered in order to fire
	int trigger_count_goal;
	// If the element fires only once
	int fire_once;
	// Number of times the element has fired
	int fired_count;
	// UUID of the first actor to trigger this element
	uint64_t uuid_cache;
	// Tick where timer will fire
	uint64_t timer_tick;
} LogicData;
static void InitData(struct Actor* actor);
static void JsonSetupData(struct Actor* actor, cJSON* file_data);
static void CleanupData(struct Actor* actor);
static int TriggerEvent(struct Actor* actor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Lets automagic this a bit more with so many elements here to handle
#define MAKE_LOGIC_ACTOR(x, flags) \
ACTOR_JSON_INIT(x); \
ACTOR_UPDATE(x); \
ACTOR_PLAYER_INTERACT(x); \
ACTOR_REMOTE_INTERACT(x); \
ACTOR_CLEANUP(x); \
ACTOR_INIT(x) { \
	actor->actor_flags = flags; \
	ACTOR_REGISTER_JSON_INIT(x); \
	ACTOR_REGISTER_UPDATE(x); \
	ACTOR_REGISTER_PLAYER_INTERACT(x); \
	ACTOR_REGISTER_REMOTE_INTERACT(x); \
	ACTOR_REGISTER_CLEANUP(x); \
	InitData(actor); \
} \
/* This assumes player interaction will trigger remote interaction too! */ \
ACTOR_PLAYER_INTERACT(x) { \
	actor->func_remote_interact(actor, player); \
} \
ACTOR_JSON_INIT(x) { \
	if (file_data == NULL) { \
		return; \
	} \
	JsonSetupData(actor,file_data); \
} \
ACTOR_CLEANUP(x) { \
	CleanupData(actor); \
}

MAKE_LOGIC_ACTOR(logic_or, ACTOR_FLAG_DOES_NOT_TICK | ACTOR_FLAG_IS_INVISIBLE);
MAKE_LOGIC_ACTOR(logic_and, ACTOR_FLAG_DOES_NOT_TICK | ACTOR_FLAG_IS_INVISIBLE);
MAKE_LOGIC_ACTOR(logic_counter, ACTOR_FLAG_DOES_NOT_TICK | ACTOR_FLAG_IS_INVISIBLE);
MAKE_LOGIC_ACTOR(logic_timer, ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TEXTBOX | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_IS_INVISIBLE);

#undef MAKE_LOGIC_ACTOR


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void InitData(struct Actor* actor)
{
	MALLOC_ACTOR_DATA(LogicData, actor->data);
	LogicData* logic_data = (LogicData*)actor->data;
	logic_data->target = NULL;
	logic_data->trigger_count = 0;
	logic_data->trigger_count_goal = 1;
	logic_data->fire_once = 0;
	logic_data->fired_count = 0;
	logic_data->uuid_cache = 0;
	logic_data->timer_tick = 0;
}

static void CleanupData(struct Actor* actor)
{
	MALLOC_ACTOR_DATA(LogicData, actor->data);
	LogicData* logic_data = (LogicData*)actor->data;
	if (logic_data->target != NULL)
		RELEASE(logic_data->target);
}

static void JsonSetupData(struct Actor* actor, cJSON* file_data)
{
	LogicData* logic_data = (LogicData*)actor->data;
	JSON_GET_STRING(logic_data->target, file_data, PROP_TARGETID, NULL);
	JSON_GET_INT(logic_data->trigger_count_goal, file_data, PROP_TRIGGERS_REQUIRED, 1);
	JSON_GET_INT(logic_data->fire_once, file_data, PROP_FIRES_ONCE, 0);
	JSON_GET_INT(logic_data->timer_tick, file_data, PROP_TIMER_DELAY, 0);
}

static int TriggerEvent(struct Actor* actor)
{
	LogicData* logic_data = (LogicData*)actor->data;

	// Incriment number of times this element was triggered
	logic_data->trigger_count += 1;
	if (logic_data->trigger_count < logic_data->trigger_count_goal)
		return FALSE;

	// Forbid firing if we only fire once
	if (logic_data->fire_once && logic_data->fired_count > 0)
		return FALSE;
	logic_data->fired_count += 1;

	// If we have nothing to target, ASSUME WE WERE SUCCESSFUL!
	if (logic_data->target == NULL)
		return TRUE;

	// Otherwise, find target actors, we can have multiple we trigger! They just all need the same id_tag.
	const struct Actor* targets[64] = { NULL };
	int found_count = FINDACTORGROUP_BYTAG(targets, 64, (const char* []) { logic_data->target });
	if (!found_count)
		return FALSE;
	for (int i = 0; i < found_count; i++)
	{
		struct Actor* target = targets[i];
		if (target == NULL)
			continue;
		if (ACTOR_HAS(target, func_remote_interact))
			target->func_remote_interact(target, actor);
	}
	return TRUE;
}


ACTOR_REMOTE_INTERACT(logic_or)
{
	// Any input is relayed ahead to the target, allowing multiple inputs to one actor.
	LogicData* logic_data = (LogicData*)actor->data;
	TriggerEvent(actor);
}
ACTOR_UPDATE(logic_or) {}


ACTOR_REMOTE_INTERACT(logic_and)
{
	// Requires two different trigger actors 
	LogicData* logic_data = (LogicData*)actor->data;

	// First triggering. Await second
	if (logic_data->uuid_cache == 0)
		logic_data->uuid_cache = other->uuid;
	// Check if it's the same actor as before
	if (logic_data->uuid_cache == other->uuid)
		return;
	// Second triggering means it's a success!
	logic_data->uuid_cache = 0; // Reset
	TriggerEvent(actor);
}
ACTOR_UPDATE(logic_and) {}


ACTOR_REMOTE_INTERACT(logic_counter)
{
	// Counts up the number of triggers, then fires if it meets the minimum needed.
	TriggerEvent(actor);
}
ACTOR_UPDATE(logic_counter) {}


ACTOR_REMOTE_INTERACT(logic_timer)
{

}
ACTOR_UPDATE(logic_timer) 
{
	LogicData* logic_data = (LogicData*)actor->data;
	
	if (logic_data->timer_tick == 0)
		return;



}