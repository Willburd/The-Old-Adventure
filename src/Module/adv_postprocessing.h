#ifndef __ADV_LUT_SHADER_HEADER__
#define __ADV_LUT_SHADER_HEADER__

#include "game_draw.h"
#include "post_processing.h"
#include <raylib.h>

void AdvLUTShaderUniforms(PostProcessingPhase phase, struct PostProcessingLayer* data, Shader* shader, RenderTexture2D* render_tex)
{
    int loc = GetShaderLocation(*shader, "lut_tex");
    switch (phase)
    {
        case post_process_world:
            SetShaderValueTexture(*shader, loc, *AssetGet_Texture(ASSET_TEXTURES"/LUTs/Neutral.png"));
            break;
        case post_process_hud:
            SetShaderValueTexture(*shader, loc, *AssetGet_Texture(ASSET_TEXTURES"/LUTs/Neutral.png"));
            break;
    }
}

void AdvDitherShaderUniforms(PostProcessingPhase phase, struct PostProcessingLayer* data, Shader* shader, RenderTexture2D* render_tex)
{
    int loc = GetShaderLocation(*shader, "dither_width");
    SetShaderValue(*shader, loc, &renderWidth, RL_SHADER_UNIFORM_INT);
    loc = GetShaderLocation(*shader, "dither_height");
    SetShaderValue(*shader, loc, &renderHeight, RL_SHADER_UNIFORM_INT);
}

#endif