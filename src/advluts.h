#ifndef __ADV_LUT_SHADER_HEADER__
#define __ADV_LUT_SHADER_HEADER__

#include "post_processing.h"
#include <raylib.h>

void AdvLUTShaderUniforms(struct PostProcessingLayer* data, RenderTexture2D* render_tex)
{
    Shader shdr = data->material->shader;
    Texture2D* tex = AssetGet_Texture(ASSET_TEXTURES"/LUTs/Neutral.png");
    Texture2D* screen_tex = &render_tex->texture;

    data->material->maps[MATERIAL_MAP_ALBEDO].texture = *screen_tex;

    int loc = GetShaderLocation(shdr, "lut_tex");
    SetShaderValueTexture(shdr, loc, *tex);
}

#endif