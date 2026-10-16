#ifndef __ACTOR_NODE_HEADER__
#define __ACTOR_NODE_HEADER__

#define NODEACTION_NEXT 0
#define NODEACTION_STOP 1
#define NODEACTION_REVERSE 2

typedef struct
{
	char* next_node_tag;
	char* prev_node_tag;
	char* alt_node_tag;
	int node_action;
	char* arrival_triggers_tag;
} NodeData;

#endif