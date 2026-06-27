#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "globals.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"

// Assets


// private header
SCENE_ACTIVATE_ROOM(Sboot);
SCENE_UPDATE(Sboot);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_INIT(Sboot)
{
	// Configure 
	SCENE_REGISTER_ACTIVATE_ROOM(Sboot);
	SCENE_REGISTER_UPDATE(Sboot);

	// Set data
	MALLOC_ACTOR_DATA(SceneData, scene->data);
	SCENEDATA_CLEAR(scene->data);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_ACTIVATE_ROOM(Sboot)
{
	// Store the current active subroom of the scene
	SceneData* data = (SceneData*)scene->data;
	data->active_room = room_index;
	data->config_flags = SCENE_CONFIG_TIMEPAUSED;
}

SCENE_UPDATE(Sboot)
{
	// Go to title
	TransferScene(scene_Sdebug, ent_title);
}
