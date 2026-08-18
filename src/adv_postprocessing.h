#ifndef __ADV_LUT_SHADER_HEADER__
#define __ADV_LUT_SHADER_HEADER__

#include "game_draw.h"
#include "post_processing.h"
#include <raylib.h>

void AdvLUTShaderUniforms(struct PostProcessingLayer* data, Shader* shader, RenderTexture2D* render_tex)
{
    int loc = GetShaderLocation(*shader, "lut_tex");
    SetShaderValueTexture(*shader, loc, *AssetGet_Texture(ASSET_TEXTURES"/LUTs/Neutral.png"));
}

void AdvDitherShaderUniforms(struct PostProcessingLayer* data, Shader* shader, RenderTexture2D* render_tex)
{
    int loc = GetShaderLocation(*shader, "dither_width");
    SetShaderValue(*shader, loc, &renderWidth, RL_SHADER_UNIFORM_INT);
    loc = GetShaderLocation(*shader, "dither_height");
    SetShaderValue(*shader, loc, &renderHeight, RL_SHADER_UNIFORM_INT);
}

#endif