#include <stdint.h>
#include "raylib.h"
#include "return_codes.h"
#include "game_update.h"
#include "game_draw.h"
#include "camera.h"
#include "globals.h"
#include "actor_factory.h"
#include "scene_entry.h"
#include "assets.h"
#include "tools.h"

const double update_rate = 40.0;
const int frame_rate = 60;
const int screenWidth = 800;
const int screenHeight = 600;

double update_ticker = 0;
uint64_t tick_counter = 0;
double seconds_counter = 0;

void game_setup();
void game_shutdown();

int main(void)
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(screenWidth, screenHeight, "The Old Adventures");
    InitAudioDevice();

    SetTargetFPS(frame_rate);
    game_setup();

    double current_prog = 0;
    double ticker_rate = 0;
    while (!WindowShouldClose())
    {
        float delta_time = GetFrameTime();

        update_ticker += delta_time;
        seconds_counter += delta_time;

        ticker_rate = 1.0 / update_rate;
        while (update_ticker >= ticker_rate)
        {
            tick_counter++;
            update_ticker -= ticker_rate;
            game_update();
        }
        game_draw(update_ticker / ticker_rate);
    }

    game_shutdown();
    CloseWindow();
    return ERR_SUCCESS;
}

void game_setup()
{
    if (world_actors == NULL) {
        perror("Actor array allocation failed!\n");
        exit(ERR_NOALLOC);
    }
    for (int i = 0; i < ACTOR_LIMIT; i++)
    {
        world_actors[i] = NULL;
    }

    // Create cameras
    cam_main = (Camera)
    {
        .position = (Vector3){0,0,0},
        .target = VEC3FORWARD,
        .fovy = 90.0f,
        .projection = CAMERA_PERSPECTIVE,
        .up = VEC3UP,
    };
    cam_hud = (Camera2D)
    {
        .target = (Vector2) {0,0},
        .offset = (Vector2) {0,0},
        .rotation = 0.0f,
        .zoom = 1.0f 
    };

    // Create asset cache
    loaded_assets = hashmap_new(sizeof(Asset), ASSET_LIMIT, 0, 0, asset_hash, asset_compare, asset_free, NULL);
    reset_global_asset_cache();

    // Enter game
    LoadScene(scene_boot, ent_title, FALSE);
}

void game_shutdown()
{
    // Clear the actors entirely
    for (int i = 0; i < ACTOR_LIMIT; i++)
    {
        struct Actor* destroy_actor = world_actors[i];
        if (!ACTOR_EXISTS(destroy_actor))
            continue;
        ACTOR_DESTROY(destroy_actor);
    }
    // Clear assets and the actor id map
    hashmap_free(loaded_assets);
}