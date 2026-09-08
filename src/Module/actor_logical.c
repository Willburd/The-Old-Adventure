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
	// id_tag of the first actor to check
	char* check_tag_A;
	// id_tag of the second actor to check
	char* check_tag_B;
} LogicData;
static void InitData(struct Actor* actor);
static void JsonSetupData(struct Actor* actor, cJSON* file_data);
static void CleanupData(struct Actor* actor);
static void TriggerEvent(struct Actor* actor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Lets automagic this a bit more with so many elements here to handle
#define MAKE_LOGIC_ACTOR(x) \
ACTOR_JSON_INIT(x); \
ACTOR_REMOTE_INTERACT(x); \
ACTOR_CLEANUP(x); \
ACTOR_INIT(x) { \
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK | ACTOR_FLAG_IS_INVISIBLE; \
	ACTOR_REGISTER_JSON_INIT(x); \
	ACTOR_REGISTER_REMOTE_INTERACT(x); \
	ACTOR_REGISTER_CLEANUP(x); \
	InitData(actor); \
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

MAKE_LOGIC_ACTOR(logic_or)
MAKE_LOGIC_ACTOR(logic_and)
MAKE_LOGIC_ACTOR(logic_xor)
MAKE_LOGIC_ACTOR(logic_counter)

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
	logic_data->trigger_count_goal = 0;
	logic_data->fire_once = 0;
	logic_data->fired_count = 0;
	logic_data->check_tag_A = NULL;
	logic_data->check_tag_B = NULL;
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

}

static void TriggerEvent(struct Actor* actor)
{
	LogicData* logic_data = (LogicData*)actor->data;
	if (logic_data->fire_once && logic_data->fired_count > 0)
		return;
	logic_data->fired_count += 1;

	// Find target actor
	struct Actor* target = FINDACTOR_BYTAG(logic_data->target);
	if (!ACTOR_EXISTS(target))
		return;
	if (ACTOR_HAS(target, func_remote_interact))
		target->func_remote_interact(target, actor);
}

ACTOR_REMOTE_INTERACT(logic_or)
{
	// Any input is relayed
	LogicData* logic_data = (LogicData*)actor->data;
	TriggerEvent(actor);
}

ACTOR_REMOTE_INTERACT(logic_and)
{
	// Requires two different triggers
	LogicData* logic_data = (LogicData*)actor->data;

}

ACTOR_REMOTE_INTERACT(logic_xor)
{
	// Requires 
	LogicData* logic_data = (LogicData*)actor->data;

}

ACTOR_REMOTE_INTERACT(logic_counter)
{
	LogicData* logic_data = (LogicData*)actor->data;
	logic_data->trigger_count += 1;
	if (logic_data->trigger_count >= logic_data->trigger_count)
		TriggerEvent(actor);
}