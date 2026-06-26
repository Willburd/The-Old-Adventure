#include <stdint.h>
#include "raylib.h"
#include "return_codes.h"
#include "game_update.h"
#include "game_draw.h"
#include "camera.h"
#include "globals.h"
#include "actor_factory.h"
#include "scene_entry.h"
#include "core_assets.h"
#include "tools.h"
#include "world_state.h"
#include "inventory.h"
#include "text_loading.h"

#define RAYMATH_USE_SIMD_INTRINSICS 1
#define PLATFORM_DESKTOP 1

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

const double update_rate = 40.0;
int frame_rate = 60;
int screenWidth = 800;
int screenHeight = 600;

int renderHeight = 240;
int renderWidth = -1;

double update_ticker = 0;
uint64_t tick_counter = 0;
double seconds_counter = 0;

static void game_setup();
static void game_shutdown();

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
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

#ifdef _DEBUG 
        if (IsKeyPressed(KEY_F2)) // Debug info
        {
            draw_debug_info = !draw_debug_info;
        }
        if (IsKeyPressed(KEY_F3)) // Uncap frames
        {
            if (frame_rate < 100) frame_rate = 9999; else frame_rate = 60;
            SetTargetFPS(frame_rate);
        }
        if (IsKeyPressed(KEY_F4)) // Debug warp
        {
            TransferScene(scene_debug, ent_title);
        }
        if (IsKeyPressed(KEY_F5)) // Reload text assets
        {
            hashmap_clear(loaded_text, FALSE);
            LoadBuiltinText();
            LoadCoreTextAssets();
        }
#endif

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

static void game_setup()
{
    if (world_actors == NULL) {
        perror("Actor array allocation failed!\n");
        exit(ERR_NOALLOC);
    }
    for (int i = 0; i < ACTOR_LIMIT; i++)
    {
        world_actors[i] = NULL;
    }

    // State
    gameplay_state = GAMESTATE_GAMEPLAY;

    // Setup renderer
    clear_background_color = BLACK;

    // Init inventory
    MALLOC(Inventory, player_inventory, ERR_NOALLOC);

    // Set worldstate up temporarily
    InitWorldState();

    // Load text data
    loaded_text = hashmap_new(sizeof(TextEntry), MAX_TEXT_ENTRIES, 0, 0, text_hash, text_compare, text_free, NULL);
    LoadBuiltinText();
    // Create asset cache
    loaded_assets = hashmap_new(sizeof(Asset), ASSET_LIMIT, 0, 0, asset_hash, asset_compare, asset_free, NULL);
    LoadCoreAssets();

    // Spawn camera
    ACTOR_FACTORY(act_camera, NULL, Vector3Zero(), QuaternionIdentity(), Vector3One(), Vector3Zero());

    // Enter game
    LoadScene(scene_boot, ent_title);
}

static void game_shutdown()
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
    UnloadAllAssets(TRUE);
    hashmap_free(loaded_text);
    hashmap_free(loaded_assets);
}