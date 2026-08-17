#ifndef __POST_PROCESSING_HEADER__
#define __POST_PROCESSING_HEADER__

#include <raylib.h>

#define MAX_POST_PROCESSING_SHADERS 24

void HandleWorldPostProcessing();
void HandleHudPostProcessing();

void RegisterWorldPostProcessShader(Material material, char* identifier);
void UnregisterWorldPostProcessShader(char* identifier);
void RegisterHudPostProcessShader(Material material, char* identifier);
void UnregisterHudPostProcessShader(char* identifier);
void ClearAllWorldPostProcessShaders();
void ClearAllHudPostProcessShaders();
void ClearAllPostProcessShaders();

#endif