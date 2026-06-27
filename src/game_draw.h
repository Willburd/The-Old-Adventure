#ifndef __GAME_DRAW_HEADER__
#define __GAME_DRAW_HEADER__

#include "raylib.h"

int draw_debug_info;

#define LIGHT_WORLD_RANGE 10000.0f
#define MAX_LIGHTS 24
typedef struct {
	Vector4 pos; 
	Vector4 col; 
} ShaderLight;
int light_count;
Vector4 world_light_positions[MAX_LIGHTS]; // XYZpos, Wradiance
Vector4 world_light_colors[MAX_LIGHTS];// XYZcolor, Walpha

#define FOG_DEFAULT_RANGE 1350.0f
#define FOG_NIGHT_MULTIPLIER 0.85f
#define FOG_DEFAULT_POWER 7.0f

float fog_distance;
float fog_power;
Vector3 fog_color;

Color clear_background_color;

RenderTexture render_tex_pre;
RenderTexture render_tex_main;
RenderTexture render_tex_post;
RenderTexture render_tex_hud;

void game_draw(double tick_percent);

void fog_set(Color col, float power, float dist);
void lighting_append_light(Vector3 pos, float radius, Color col, float influence);

void shader_update_fog(Shader shader);
void shader_update_lights(Shader shader);

#endif