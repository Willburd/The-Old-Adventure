#include "post_processing.h"
#include "game_draw.h"
#include "camera.h"
#include "tools.h"

// Processing layers
int world_post_processing_shader_count = 0;
struct PostProcessingLayer* world_post_processing_shaders[MAX_POST_PROCESSING_SHADERS] = { 0 };
int hud_post_processing_shader_count = 0;
struct PostProcessingLayer* hud_post_processing_shaders[MAX_POST_PROCESSING_SHADERS] = { 0 };

// Input and final buffers
RenderTexture2D render_tex_postworld = { 0 };
RenderTexture2D render_tex_posthud = { 0 };

// Leapfrog layers
static RenderTexture2D render_tex_leap = { 0 };
static RenderTexture2D render_tex_frog = { 0 };
static RenderTexture2D* render_tex_current = NULL;
static RenderTexture2D* render_tex_other = NULL;

static inline void HandlePostProcessing(RenderTexture2D* tex, Rectangle src, Rectangle dest, Vector2 org, struct PostProcessingLayer* mat_arr[], int arr_limit)
{
	for (int i = 0; i < arr_limit; i++)
	{
		if (mat_arr[i] == NULL)
			continue;
		struct PostProcessingLayer* current_layer = mat_arr[i];

		// Render to the current swap texture
		BeginTextureMode(*render_tex_current);
		ClearBackground((Color) { 0, 0, 0, 0 }); // Full Clear
		BeginMode2D(cam_hud);
		{
			Shader* set_shader = &current_layer->material->shader;
			BeginShaderMode(*set_shader);
			if (current_layer->func_uniforms != NULL)
				current_layer->func_uniforms(current_layer, set_shader, render_tex_current);
			if (i == 0) // Use input texture if we're the first one!
				DrawTexturePro(tex->texture, src, dest, org, 0, WHITE);
			else
				DrawTexturePro(render_tex_other->texture, src, dest, org, 0, WHITE);
			EndShaderMode();
		}
		EndMode2D();
		EndTextureMode();

		// Swap render textures
		RenderTexture* temp = render_tex_current;
		render_tex_current = render_tex_other;
		render_tex_other = temp;
	}

	// Draw back to input
	BeginTextureMode(*tex);
	ClearBackground((Color) { 0, 0, 0, 0 }); // Full Clear
	BeginMode2D(cam_hud);
	{
		DrawTexturePro(render_tex_other->texture, src, dest, org, 0, WHITE);
	}
	EndMode2D();
	EndTextureMode();
}

void HandleWorldPostProcessing(RenderTexture2D* tex, Rectangle src, Rectangle dest, Vector2 org)
{
	if (world_post_processing_shader_count == 0)
		return;
	HandlePostProcessing(tex, src, dest, org, world_post_processing_shaders, world_post_processing_shader_count);
}

void HandleHudPostProcessing(RenderTexture2D* tex, Rectangle src, Rectangle dest, Vector2 org)
{
	if (hud_post_processing_shader_count == 0)
		return;
	HandlePostProcessing(tex, src, dest, org, hud_post_processing_shaders, hud_post_processing_shader_count);
}

static inline int RegisterPostProcessShader(struct PostProcessingLayer* layer_array[], int layer_limit, Material* material, char* identifier, void (*uniforms_function)(struct PostProcessingLayer* data))
{
	for (int i = 0; i <= layer_limit; i++)
	{
		if (i < layer_limit && layer_array[i] != NULL)
			continue;
		// Register to the first free slot
		MALLOC_SET(struct PostProcessingLayer, layer_array[i], NULL);
		struct PostProcessingLayer* current_layer = layer_array[i];
		CHAR_STR_COPY(current_layer->id, identifier, NULL);
		current_layer->material = material;
		current_layer->func_uniforms = uniforms_function;
		return (i == layer_limit);
	}
	return FALSE;
}

void RegisterWorldPostProcessShader(Material* material, char* identifier, void (*uniforms_function)(struct PostProcessingLayer* data, RenderTexture2D* render_tex))
{
	if (RegisterPostProcessShader(world_post_processing_shaders, world_post_processing_shader_count, material, identifier, uniforms_function))
		world_post_processing_shader_count++;
}

void RegisterHudPostProcessShader(Material* material, char* identifier, void (*uniforms_function)(struct PostProcessingLayer* data, RenderTexture2D* render_tex))
{
	if (RegisterPostProcessShader(hud_post_processing_shaders, hud_post_processing_shader_count, material, identifier, uniforms_function))
		hud_post_processing_shader_count++;
}

static inline void UnregisterPostProcessShader(struct PostProcessingLayer* data)
{
	RELEASE(data->id); // allocated string
	data->material = NULL; // Not ours
	data->func_uniforms = NULL; // Not ours
}

void UnregisterWorldPostProcessShader(char* identifier)
{
	for (int i = 0; i < world_post_processing_shader_count; i++)
	{
		if (world_post_processing_shaders[i] == NULL)
			continue;
		if (world_post_processing_shaders[i]->id != identifier)
			continue;
		UnregisterPostProcessShader(world_post_processing_shaders[i]);
		RELEASE(world_post_processing_shaders[i]);
		return;
	}
}

void UnregisterHudPostProcessShader(char* identifier)
{
	for (int i = 0; i < hud_post_processing_shader_count; i++)
	{
		if (hud_post_processing_shaders[i] == NULL)
			continue;
		if (hud_post_processing_shaders[i]->id != identifier)
			continue;
		UnregisterPostProcessShader(hud_post_processing_shaders[i]);
		RELEASE(hud_post_processing_shaders[i]);
		return;
	}
}

void ClearAllWorldPostProcessShaders()
{
	for (int i = 0; i < world_post_processing_shader_count; i++)
	{
		if (world_post_processing_shaders[i] == NULL)
			continue;
		UnregisterPostProcessShader(world_post_processing_shaders[i]);
		RELEASE(world_post_processing_shaders[i]);
	}
	world_post_processing_shader_count = 0;
}

void ClearAllHudPostProcessShaders()
{
	for (int i = 0; i < hud_post_processing_shader_count; i++)
	{
		if (hud_post_processing_shaders[i] == NULL)
			continue;
		UnregisterPostProcessShader(hud_post_processing_shaders[i]);
		RELEASE(hud_post_processing_shaders[i]);
	}
	hud_post_processing_shader_count = 0;
}

void ClearAllPostProcessShaders()
{
	ClearAllWorldPostProcessShaders();
	ClearAllHudPostProcessShaders();
}

void LoadPostProcessingTextures()
{
	render_tex_postworld = LoadRenderTexture(renderWidth, renderHeight);
	render_tex_posthud = LoadRenderTexture(renderWidth, renderHeight);
	// Internals
	render_tex_leap = LoadRenderTexture(renderWidth, renderHeight);
	render_tex_frog = LoadRenderTexture(renderWidth, renderHeight);
	render_tex_current = &render_tex_leap; // Reset our chain too
	render_tex_other = &render_tex_frog; // Reset our chain too
}

void UnloadPostProcessingTextures()
{
	if (IsRenderTextureValid(render_tex_postworld))
		UnloadRenderTexture(render_tex_postworld);
	if (IsRenderTextureValid(render_tex_posthud))
		UnloadRenderTexture(render_tex_posthud);
	// Internals
	if (IsRenderTextureValid(render_tex_leap))
		UnloadRenderTexture(render_tex_leap);
	if (IsRenderTextureValid(render_tex_frog))
		UnloadRenderTexture(render_tex_frog);
	render_tex_current = NULL;
	render_tex_other = NULL;
}
