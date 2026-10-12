#include "actor_library.h"
#include "module_main.h"

const char* ModuleGameName()
{
    return "The Old Adventures - An Engine For Nostalgia"; // TODO - Your game name here
}

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