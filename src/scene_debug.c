#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "actor_factory.h"
#include "actor_scene.h"
#include "scene_entry.h"
#include "game_draw.h"
#include "camera.h"

void scene_debug_destroy(struct Actor* scene);
void scene_debug_update(struct Actor* scene);
void scene_debug_drawhud(struct Actor* scene, double tick_percent);

void scene_debug_init(struct Actor* scene)
{
	// Configure scene
	scene->func_destroy = scene_debug_destroy;
	scene->func_update = scene_debug_update;
	scene->func_postdrawhud = scene_debug_drawhud;
	MALLOC_ACTOR_DATA(SceneData, scene->data);

	// Set sky
	cam_main.position = (Vector3){ 0, 2, 0 };
	clear_background_color = BLACK;

	// Choose a scene to warp to list

	// Debug info actor
	ACTOR_FACTORY(act_debug, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
}

void scene_debug_update(struct Actor* scene)
{

}

void scene_debug_drawhud(struct Actor* scene, double tick_percent)
{
	DrawText("Load Scene:", 100, 30, 20, WHITE);
}

void scene_debug_destroy(struct Actor* scene)
{

}
