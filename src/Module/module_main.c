#include "module_main.h"

void ModuleInit()
{

}

void ModuleLateInit()
{

}

void ModuleInternalUpdate()
{
#ifdef _DEBUG
    if (IsKeyPressed(KEY_F7)) // Misc debug
    {

    }
#endif
}

void ModuleGameUpdate()
{

}

void ModuleCleanup()
{

}

void ModuleExtendShaderDefaultUniforms(Shader shader)
{

}

void ModuleExtendShaderFogUniforms(Shader shader)
{

}

void ModuleExtendShaderLightUniforms(Shader shader)
{

}