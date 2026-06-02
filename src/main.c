#include <stdint.h>
#include "raylib.h"
#include "return_codes.h"
#include "game_update.h"
#include "game_draw.h"
#include "globals.h"
#include "actor.h"
#include "assets.h"
#include "scene_entry.h"

const double update_rate = 20.0;
const int screenWidth = 800;
const int screenHeight = 600;

double update_ticker = 0;
uint64_t tick_counter = 0;
double seconds_counter = 0;

void game_setup();
void game_shutdown();

int main(void)
{
    InitWindow(screenWidth, screenHeight, "The Old Adventures");
    InitAudioDevice();

    SetTargetFPS(60);
    game_setup();

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
        float tick_percent = update_ticker / ticker_rate;
        game_draw(tick_percent);
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

    // Create asset cache
    loaded_assets = hashmap_new(sizeof(Asset), ASSET_LIMIT, 0, 0, asset_hash, asset_compare, asset_free, NULL);
    reset_global_asset_cache();

    // Enter game
    LoadScene(game_start_scene, titleentrance);
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