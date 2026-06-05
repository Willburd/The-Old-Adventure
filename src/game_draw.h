#ifndef GAME_DRAW_HEADER
#define GAME_DRAW_HEADER

#include "raylib.h"

int draw_debug_info;

Color clear_background_color;

RenderTexture render_tex_pre;
RenderTexture render_tex_main;
RenderTexture render_tex_post;
RenderTexture render_tex_hud;

void game_draw(double tick_percent);

#endif