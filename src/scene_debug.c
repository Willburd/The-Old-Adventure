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

typedef struct
{
	int menu_index;
} SceneData_Debug;

void scene_debug_init(struct Actor* scene)
{
	// Configure scene
	scene->func_destroy = scene_debug_destroy;
	scene->func_update = scene_debug_update;
	scene->func_postdrawhud = scene_debug_drawhud;

	// Set data
	MALLOC_ACTOR_DATA(SceneData_Debug, scene->data);
	SceneData_Debug* our_data = (SceneData_Debug*)scene->data;
	our_data->menu_index = 0;

	// Set sky
	cam_main.position = (Vector3){ 0, 2, 0 };
	clear_background_color = BLACK;

	// Debug info actor
	ACTOR_FACTORY(act_debug, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());
}

void scene_debug_update(struct Actor* scene)
{
	SceneData_Debug* our_data = (SceneData_Debug*)scene->data;
	our_data->menu_index += 1;

}

void scene_debug_drawhud(struct Actor* scene, double tick_percent)
{
	SceneData_Debug* our_data = (SceneData_Debug*)scene->data;

	DrawText(TextFormat("Load Scene [%i]:", our_data->menu_index), 100, 30, 20, WHITE);


}

void scene_debug_destroy(struct Actor* scene)
{

}
