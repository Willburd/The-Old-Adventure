#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"

/*******************************************
			Gameplay Debuging Room
********************************************/

// Assets

// private header
SCENE_PRELOADASSETS(test);
SCENE_ACTIVATE_ROOM(test);
SCENE_LIGHTNODES(test);
SCENE_DRAWWORLD(test);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_INIT(test)
{
	SCENE_REGISTER_PRELOADASSETS(test);
	SCENE_REGISTER_ACTIVATE_ROOM(test);
	SCENE_REGISTER_LIGHTNODES(test);
	SCENE_REGISTER_DRAWWORLD(test);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCENE_PRELOADASSETS(test)
{

}

SCENE_ACTIVATE_ROOM(test)
{
	// Actor spawns

	// Function testing

}

SCENE_LIGHTNODES(test)
{

}

SCENE_DRAWWORLD(test)
{

}