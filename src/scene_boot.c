#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"

void scene_boot_destroy(struct Actor* scene);

void scene_boot_init(struct Actor* scene)
{
	// Configure scene
	scene->func_destroy = scene_boot_destroy;
}

void scene_boot_destroy(struct Actor* scene)
{

}
