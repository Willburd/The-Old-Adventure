#ifndef __POST_PROCESSING_HEADER__
#define __POST_PROCESSING_HEADER__

#include <raylib.h>

typedef enum {
	post_process_world,
	post_process_hud,
	post_process_final
} PostProcessingPhase;

#define MAX_POST_PROCESSING_SHADERS 128

struct PostProcessingLayer;
struct PostProcessingLayer {
	char* id;
	Material* material;
	unsigned int priority; // Lower numbers are inserted into the array first.
	void (*func_uniforms)(PostProcessingPhase phase, struct PostProcessingLayer* data, Shader* shader, RenderTexture2D* render_tex);
};

RenderTexture2D render_tex_postworld;
RenderTexture2D render_tex_posthud;
RenderTexture2D render_tex_postfinal;

void HandleWorldPostProcessing(RenderTexture2D* tex, Rectangle src, Rectangle dest, Vector2 org);
void HandleHudPostProcessing(RenderTexture2D* tex, Rectangle src, Rectangle dest, Vector2 org);
void HandleFinalPostProcessing(RenderTexture2D* tex, Rectangle src, Rectangle dest, Vector2 org);

void RegisterWorldPostProcessShader(Material* material, char* identifier, unsigned int priority, void (*uniforms_function)(PostProcessingPhase phase, struct PostProcessingLayer* data, RenderTexture2D* render_tex));
void UnregisterWorldPostProcessShader(char* identifier);

void RegisterHudPostProcessShader(Material* material, char* identifier, unsigned int priority, void (*uniforms_function)(PostProcessingPhase phase, struct PostProcessingLayer* data, RenderTexture2D* render_tex));
void UnregisterHudPostProcessShader(char* identifier);

void RegisterFinalPostProcessShader(Material* material, char* identifier, unsigned int priority, void (*uniforms_function)(PostProcessingPhase phase, struct PostProcessingLayer* data, RenderTexture2D* render_tex));
void UnregisterFinalPostProcessShader(char* identifier);

void UnregisterAllWorldPostProcessShaders();
void UnregisterAllHudPostProcessShaders();
void UnregisterAllFinalPostProcessShaders();
void UnregisterAllPostProcessShaders();

void LoadPostProcessingTextures();
void UnloadPostProcessingTextures();

#endif