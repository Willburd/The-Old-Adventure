#ifndef __POST_PROCESSING_HEADER__
#define __POST_PROCESSING_HEADER__

#include <raylib.h>

#define MAX_POST_PROCESSING_SHADERS 128

struct PostProcessingLayer;
struct PostProcessingLayer {
	char* id;
	Material* material;
	void (*func_uniforms)(struct PostProcessingLayer* data, RenderTexture2D* render_tex);
};

RenderTexture2D render_tex_postworld;
RenderTexture2D render_tex_posthud;

void HandleWorldPostProcessing(RenderTexture2D* tex, Rectangle src, Rectangle dest, Vector2 org);
void HandleHudPostProcessing(RenderTexture2D* tex, Rectangle src, Rectangle dest, Vector2 org);

void RegisterWorldPostProcessShader(Material* material, char* identifier, void (*uniforms_function)(struct PostProcessingLayer* data, RenderTexture2D* render_tex));
void UnregisterWorldPostProcessShader(char* identifier);
void RegisterHudPostProcessShader(Material* material, char* identifier, void (*uniforms_function)(struct PostProcessingLayer* data, RenderTexture2D* render_tex));
void UnregisterHudPostProcessShader(char* identifier);
void ClearAllWorldPostProcessShaders();
void ClearAllHudPostProcessShaders();
void ClearAllPostProcessShaders();

void LoadPostProcessingTextures();
void UnloadPostProcessingTextures();

#endif