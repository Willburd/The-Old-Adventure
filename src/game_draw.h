#ifndef __GAME_DRAW_HEADER__
#define __GAME_DRAW_HEADER__

#include "assets.h"
#include "raylib.h"
#include "rlgl.h"

int draw_debug_info;
int draw_collider_info;

#define LIGHT_WORLD_RANGE 10000.0f
#define MAX_LIGHTS 48

#define FOG_DEFAULT_RANGE 1350.0f
#define FOG_NIGHT_MULTIPLIER 0.85f
#define FOG_DEFAULT_POWER 7.0f

#define STANDARD_SHADER_MATERIAL(name, material_path, act) Material* name = AssetGet_Material(material_path);ShaderUpdateDefaultUniforms(name->shader, act);ShaderUpdateFogUniforms(name->shader);ShaderUpdateLightUniforms(name->shader);

#define RENDER_LAYER_SIZE 4096

float fog_distance;
float fog_power;
Vector3 fog_color;

Color clear_background_color;

// Not autocleared, should be updated during actor UPDATE functions and not draw, and only for changes.
const int renderlayers_enabled;
Vector2 renderlayer_pos_background;
RenderTexture2D render_tex_background;
Vector2 renderlayer_pos_tilemap;
RenderTexture2D render_tex_tilemap;
Vector2 renderlayer_pos_foreground;
RenderTexture2D render_tex_foreground;

void game_draw(double tick_percent);

void fog_set(Color col, float power, float dist);
/// Lights are updated each frame and do not persist between them. Lights need to be "appended" to the light list each update to render. This is easier than juggling light references when wanting to animate lights.
void AppendLight(Vector3 pos, float radius, Color col, float influence);
void ResetLightCount();
int GetLightCount();

void ShaderUpdateDefaultUniforms(Shader shader, struct Actor* actor);
void ShaderUpdateFogUniforms(Shader shader);
void ShaderUpdateLightUniforms(Shader shader);

/// Extended mesh drawing
void ToaDrawMesh(Asset* model_asset, int mesh_index, Material material, Matrix matrix, int show_backface);
void DrawAllModelMeshes(struct Actor* actor, char* model_path, char* material_paths[]);

void LoadRenderTextures();
void UnloadRenderTextures();
void LoadRenderLayers();
void UnloadRenderLayers();

#endif