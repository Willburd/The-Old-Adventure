#include "post_processing.h"
#include "game_draw.h"
#include "camera.h"
#include "tools.h"

// Processing layers
int world_post_processing_shader_count = 0;
struct PostProcessingLayer* world_post_processing_shaders[MAX_POST_PROCESSING_SHADERS] = { 0 };
int hud_post_processing_shader_count = 0;
struct PostProcessingLayer* hud_post_processing_shaders[MAX_POST_PROCESSING_SHADERS] = { 0 };
int final_post_processing_shader_count = 0;
struct PostProcessingLayer* final_post_processing_shaders[MAX_POST_PROCESSING_SHADERS] = { 0 };

// Input and final buffers
RenderTexture2D render_tex_postworld = { 0 };
RenderTexture2D render_tex_posthud = { 0 };
RenderTexture2D render_tex_postfinal = { 0 };

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

void HandleFinalPostProcessing(RenderTexture2D* tex, Rectangle src, Rectangle dest, Vector2 org)
{
	if (final_post_processing_shader_count == 0)
		return;
	HandlePostProcessing(tex, src, dest, org, final_post_processing_shaders, final_post_processing_shader_count);
}

static inline struct PostProcessingLayer* CreateLayer(Material* material, char* identifier, unsigned int priority, void (*uniforms_function)(struct PostProcessingLayer* data))
{
	MALLOC(struct PostProcessingLayer, current_layer, NULL);
	CHAR_STR_COPY(current_layer->id, identifier, NULL);
	current_layer->material = material;
	current_layer->func_uniforms = uniforms_function;
	current_layer->priority = priority;
	return current_layer;
}

static inline int RegisterPostProcessShader(struct PostProcessingLayer* layer_array[], int layer_limit, Material* material, char* identifier, unsigned int priority, void (*uniforms_function)(struct PostProcessingLayer* data))
{
	// If no layers exist, just add it.
	if (layer_limit == 0)
	{
		layer_array[0] = CreateLayer(material, identifier, priority, uniforms_function);
		return 1;
	}

	// Store the current layers
	struct PostProcessingLayer* temp_arr[MAX_POST_PROCESSING_SHADERS] = { NULL };
	for (int i = 0; i <= layer_limit; i++)
	{
		if (i == layer_limit)
		{
			temp_arr[i] = CreateLayer(material, identifier, priority, uniforms_function);
			layer_array[i] = NULL;
			break;
		}
		temp_arr[i] = layer_array[i];
		layer_array[i] = NULL;
	}

	// Add each layer from lowest priority to highest
	int write_index = 0;
	while (TRUE)
	{
		int found_lowest_index = -1;
		unsigned int find_lowest_priority = UINT32_MAX;
		for (int i = 0; i <= layer_limit; i++)
		{
			if (temp_arr[i] == NULL)
				continue;
			if (temp_arr[i]->priority > find_lowest_priority)
				continue;
			// Yoink the index...
			find_lowest_priority = temp_arr[i]->priority;
			found_lowest_index = i;
		}
		// ...if we found nothing leave it...
		if (found_lowest_index == -1)
			return write_index;
		// ...otherwise write it to the output list.
		layer_array[write_index++] = temp_arr[found_lowest_index];
		temp_arr[found_lowest_index] = NULL;
	}
}

void RegisterWorldPostProcessShader(Material* material, char* identifier, unsigned int priority, void (*uniforms_function)(struct PostProcessingLayer* data, RenderTexture2D* render_tex))
{
	world_post_processing_shader_count = RegisterPostProcessShader(world_post_processing_shaders, world_post_processing_shader_count, material, identifier, priority, uniforms_function);
}

void RegisterHudPostProcessShader(Material* material, char* identifier, unsigned int priority, void (*uniforms_function)(struct PostProcessingLayer* data, RenderTexture2D* render_tex))
{
	hud_post_processing_shader_count = RegisterPostProcessShader(hud_post_processing_shaders, hud_post_processing_shader_count, material, identifier, priority, uniforms_function);
}

void RegisterFinalPostProcessShader(Material* material, char* identifier, unsigned int priority, void (*uniforms_function)(struct PostProcessingLayer* data, RenderTexture2D* render_tex))
{
	final_post_processing_shader_count = RegisterPostProcessShader(final_post_processing_shaders, final_post_processing_shader_count, material, identifier, priority, uniforms_function);
}

static inline void UnregisterPostProcessShader(struct PostProcessingLayer* data)
{
	RELEASE(data->id); // allocated string
	data->material = NULL; // Not ours
	data->func_uniforms = NULL; // Not ours
}

#define UNREGISTER_LOOP(arr, cont) \
for (int i = 0; i < cont; i++) \
{ \
	if (arr[i] == NULL) \
		continue; \
	if (arr[i]->id != identifier) \
		continue; \
	UnregisterPostProcessShader(arr[i]); \
	RELEASE(arr[i]); \
	return; \
}
void UnregisterWorldPostProcessShader(char* identifier)
{
	UNREGISTER_LOOP(world_post_processing_shaders, world_post_processing_shader_count);
}

void UnregisterHudPostProcessShader(char* identifier)
{
	UNREGISTER_LOOP(hud_post_processing_shaders, hud_post_processing_shader_count);
}

void UnregisterFinalPostProcessShader(char* identifier)
{
	UNREGISTER_LOOP(final_post_processing_shaders, final_post_processing_shader_count);
}
#undef UNREGISTER_LOOP

#define CLEAR_LOOP(arry, cont) \
for (int i = 0; i < cont; i++) \
{ \
	if (arry[i] == NULL) \
		continue; \
	UnregisterPostProcessShader(arry[i]); \
	RELEASE(arry[i]); \
} \
cont = 0;
void ClearAllWorldPostProcessShaders()
{
	CLEAR_LOOP(world_post_processing_shaders, world_post_processing_shader_count);
}

void ClearAllHudPostProcessShaders()
{
	CLEAR_LOOP(hud_post_processing_shaders, hud_post_processing_shader_count);
}

void ClearAllFinalPostProcessShaders()
{
	CLEAR_LOOP(final_post_processing_shaders, final_post_processing_shader_count);
}
#undef CLEAR_LOOP

void ClearAllPostProcessShaders()
{
	ClearAllWorldPostProcessShaders();
	ClearAllHudPostProcessShaders();
	ClearAllFinalPostProcessShaders();
}

void LoadPostProcessingTextures()
{
	render_tex_postworld = LoadRenderTexture(renderWidth, renderHeight);
	render_tex_posthud = LoadRenderTexture(renderWidth, renderHeight);
	render_tex_postfinal = LoadRenderTexture(renderWidth, renderHeight);
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
	if (IsRenderTextureValid(render_tex_postfinal))
		UnloadRenderTexture(render_tex_postfinal);
	// Internals
	if (IsRenderTextureValid(render_tex_leap))
		UnloadRenderTexture(render_tex_leap);
	if (IsRenderTextureValid(render_tex_frog))
		UnloadRenderTexture(render_tex_frog);
	render_tex_current = NULL;
	render_tex_other = NULL;
}
