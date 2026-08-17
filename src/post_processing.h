#ifndef __POST_PROCESSING_HEADER__
#define __POST_PROCESSING_HEADER__

#include <raylib.h>

#define MAX_POST_PROCESSING_SHADERS 128

RenderTexture render_tex_postworld;
RenderTexture render_tex_posthud;

void HandleWorldPostProcessing(RenderTexture* tex, Rectangle src, Rectangle dest, Vector2 org);
void HandleHudPostProcessing(RenderTexture* tex, Rectangle src, Rectangle dest, Vector2 org);

void RegisterWorldPostProcessShader(Material* material, char* identifier, void* uniforms_function);
void UnregisterWorldPostProcessShader(char* identifier);
void RegisterHudPostProcessShader(Material* material, char* identifier, void* uniforms_function);
void UnregisterHudPostProcessShader(char* identifier);
void ClearAllWorldPostProcessShaders();
void ClearAllHudPostProcessShaders();
void ClearAllPostProcessShaders();

void LoadPostProcessingTextures();
void UnloadPostProcessingTextures();

#endif