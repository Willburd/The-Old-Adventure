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
    Vector2 res = (Vector2){ renderWidth, renderHeight };
    int loc = GetShaderLocation(*shader, "uRenderResolution");
    SetShaderValue(*shader, loc, &res, RL_SHADER_UNIFORM_VEC2);
}

#endif