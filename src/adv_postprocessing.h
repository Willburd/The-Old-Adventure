#ifndef __ADV_LUT_SHADER_HEADER__
#define __ADV_LUT_SHADER_HEADER__

#include "game_draw.h"
#include "post_processing.h"
#include <raylib.h>

void AdvLUTShaderUniforms(struct PostProcessingLayer* data, Shader* shader, RenderTexture2D* render_tex)
{
    Texture2D* tex = AssetGet_Texture(ASSET_TEXTURES"/LUTs/Neutral.png");
    Texture2D* screen_tex = &render_tex->texture;

    int loc = GetShaderLocation(*shader, "lut_tex");
    SetShaderValueTexture(*shader, loc, *tex);
}

void AdvDitherShaderUniforms(struct PostProcessingLayer* data, Shader* shader, RenderTexture2D* render_tex)
{

}

#endif