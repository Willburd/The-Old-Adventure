#include "post_processing.h"

int world_post_processing_shader_count = 0;
Material world_post_processing_shaders[MAX_POST_PROCESSING_SHADERS] = { 0 };
int hud_post_processing_shader_count = 0;
Material hud_post_processing_shaders[MAX_POST_PROCESSING_SHADERS] = { 0 };


void HandleWorldPostProcessing()
{

}

void HandleHudPostProcessing()
{

}

void RegisterWorldPostProcessShader(Material material, char* identifier)
{

}

void UnregisterWorldPostProcessShader(char* identifier)
{

}

void RegisterHudPostProcessShader(Material material, char* identifier)
{

}

void UnregisterHudPostProcessShader(char* identifier)
{

}

void ClearAllWorldPostProcessShaders()
{

}

void ClearAllHudPostProcessShaders()
{

}

void ClearAllPostProcessShaders()
{

}
