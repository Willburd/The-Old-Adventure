#include "../actor_factory.h"
#include "../tools.h"
#include "../collision.h"
#include "../game_draw.h"
#include "actor_node.h"
#include "json_properties.h"

// Assets

// private header
ACTOR_JSON_INIT(node);
ACTOR_CLEANUP(node);
ACTOR_REMOTE_INTERACT(node);
ACTOR_POSTDRAWWORLD(node);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(node)
{
	ACTOR_REGISTER_JSON_INIT(node);
	ACTOR_REGISTER_REMOTE_INTERACT(node);
	ACTOR_REGISTER_CLEANUP(node);
	ACTOR_REGISTER_POSTDRAWWORLD(node);

	// Set data
	MALLOC_ACTOR_DATA(NodeData, actor->data);
	NodeData* node_data = (NodeData*)actor->data;
	node_data->next_node_tag = NULL;
	node_data->prev_node_tag = NULL;
	node_data->alt_node_tag = NULL;
	node_data->node_action = NODEACTION_NEXT;
	node_data->arrival_triggers_tag = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ACTOR_JSON_INIT(node)
{
	if (file_data == NULL)
		return;

	NodeData* node_data = (NodeData*)actor->data;
	JSON_GET_STRING(node_data->next_node_tag, file_data, PROP_NEXTNODE, NULL);
	JSON_GET_STRING(node_data->prev_node_tag, file_data, PROP_PREVNODE, NULL);
	JSON_GET_STRING(node_data->alt_node_tag, file_data, PROP_ALTNODE, NULL);
	JSON_GET_INT(node_data->node_action, file_data, PROP_NODEACTION, NODEACTION_NEXT);
	JSON_GET_STRING(node_data->arrival_triggers_tag, file_data, PROP_TARGETID, NULL);
}

ACTOR_REMOTE_INTERACT(node)
{
	// swap next and alt
	NodeData* node_data = (NodeData*)actor->data;
	char* old_next = node_data->next_node_tag;
	node_data->next_node_tag = node_data->alt_node_tag;
	node_data->alt_node_tag = old_next;
}

ACTOR_CLEANUP(node)
{
	NodeData* node_data = (NodeData*)actor->data;
	if(node_data->next_node_tag != NULL)
		RELEASE(node_data->next_node_tag);
	if (node_data->prev_node_tag != NULL)
		RELEASE(node_data->prev_node_tag);
	if (node_data->alt_node_tag != NULL)
		RELEASE(node_data->alt_node_tag);
	if (node_data->arrival_triggers_tag != NULL)
		RELEASE(node_data->arrival_triggers_tag);
}

ACTOR_POSTDRAWWORLD(node)
{
	if (!draw_debug_info)
		return;
	DrawSphere(actor->position, 0.25f, BLUE);

	NodeData* node_data = (NodeData*)actor->data;
	if (node_data->next_node_tag != NULL)
	{
		struct Actor* next = FINDACTOR_BYTAG(node_data->next_node_tag);
		if (next != NULL)
		{
			DrawLine3D(actor->position, next->position, GREEN);
			Vector3 point = Vector3Lerp(actor->position, next->position, 0.04f);
			DrawSphere(point, 0.15f, GREEN);
		}
	}
	if (node_data->prev_node_tag != NULL)
	{
		struct Actor* prev = FINDACTOR_BYTAG(node_data->prev_node_tag);
		if (prev != NULL)
		{
			DrawLine3D(actor->position, prev->position, RED);
			Vector3 point = Vector3Lerp(actor->position, prev->position, 0.04f);
			DrawSphere(point, 0.15f, RED);
		}
	}
	if (node_data->alt_node_tag != NULL)
	{
		struct Actor* alt = FINDACTOR_BYTAG(node_data->alt_node_tag);
		if (alt != NULL)
		{
			DrawLine3D(actor->position, alt->position, YELLOW);
			Vector3 point = Vector3Lerp(actor->position, alt->position, 0.04f);
			DrawSphere(point, 0.15f, YELLOW);
		}
	}
}