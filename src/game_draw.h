#ifndef GAME_DRAW_HEADER
#define GAME_DRAW_HEADER

#include "raylib.h"

int draw_debug_info;

#define MAX_LIGHTS 16
typedef struct {
	Vector4 pos; // XYZpos, Wradiance
	Vector4 col; // XYZcolor, Walpha
} ShaderLight;
int light_count;
ShaderLight world_lights[MAX_LIGHTS];

#define FOG_DEFAULT_RANGE 320.0f
#define FOG_DEFAULT_POWER 4.0f

float fog_distance;
float fog_power;
Vector4 fog_color;

Color clear_background_color;

RenderTexture render_tex_pre;
RenderTexture render_tex_main;
RenderTexture render_tex_post;
RenderTexture render_tex_hud;

void game_draw(double tick_percent);

void fog_set(Color col, float power, float dist);
void lighting_append_light(Vector3 pos, float radius, Color col);

void shader_update_fog(Shader shader);
void shader_update_camera_pos(Shader shader);
void shader_update_lights(Shader shader);

#endif