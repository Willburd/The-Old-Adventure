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
ACTOR_POSTDRAWWORLD(node);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(node)
{
	ACTOR_REGISTER_JSON_INIT(node);
	ACTOR_REGISTER_CLEANUP(node);
	ACTOR_REGISTER_POSTDRAWWORLD(node);

	// Set data
	MALLOC_ACTOR_DATA(NodeData, actor->data);
	NodeData* node_data = (NodeData*)actor->data;
	node_data->next_node_tag = NULL;
	node_data->prev_node_tag = NULL;
	node_data->alt_node_tag = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ACTOR_JSON_INIT(node)
{
	if (file_data == NULL)
		return;

	NodeData* node_data = (NodeData*)actor->data;
	cJSON* get_str = cJSON_GetObjectItem(file_data, PROP_NEXTNODE);
	if (cJSON_IsString(get_str))
	{
		char* str = get_str->valuestring;
		CHAR_STR_COPY(node_data->next_node_tag, str, NULL);
	}

	get_str = cJSON_GetObjectItem(file_data, PROP_PREVNODE);
	if (cJSON_IsString(get_str))
	{
		char* str = get_str->valuestring;
		CHAR_STR_COPY(node_data->prev_node_tag, str, NULL);
	}
	
	get_str = cJSON_GetObjectItem(file_data, PROP_ALTNODE);
	if (cJSON_IsString(get_str))
	{
		char* str = get_str->valuestring;
		CHAR_STR_COPY(node_data->alt_node_tag, str, NULL);
	}
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