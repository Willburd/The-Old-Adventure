#ifndef GAME_DRAW_HEADER
#define GAME_DRAW_HEADER

#include "raylib.h"

Color clear_background_color;

RenderTexture render_tex_pre;
RenderTexture render_tex_main;
RenderTexture render_tex_post;

void game_draw(double tick_percent);

#endif