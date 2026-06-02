#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "globals.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"

void scene_boot_destroy(struct Actor* scene);
void scene_boot_update(struct Actor* scene);

void scene_boot_init(struct Actor* scene)
{
	// Configure 
	scene->func_update = scene_boot_update;
	scene->func_destroy = scene_boot_destroy;
}

void scene_boot_update(struct Actor* scene)
{
	// Go to title
	LoadScene(scene_debug, ent_title, TRUE);
}

void scene_boot_destroy(struct Actor* scene)
{

}
