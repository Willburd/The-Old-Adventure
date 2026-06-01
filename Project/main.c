#include "raylib.h"
#include "return_codes.h"
#include "game_update.h"
#include "game_draw.h"
#include "globals.h"

const double update_rate = 20.0;
const int screenWidth = 800;
const int screenHeight = 600;

double update_ticker = 0;
__int64 tick_counter = 0;
double seconds_counter = 0;

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

        game_draw(delta_time);
    }

    game_shutdown();
    CloseWindow();
    return ERR_SUCCESS;
}