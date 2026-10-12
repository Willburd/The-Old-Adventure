#ifndef __MODULE_MAIN_HEADER__
#define __MODULE_MAIN_HEADER__

#include <raylib.h>

// Called during game's initilization in main()
void ModuleInit();
// Called at the end of game's initilization in main() once core assets have finished loading
void ModuleLateInit();
// Called during each update loop before checking if the gameupdate tick should happen
void ModuleInternalUpdate();
// Called during each gametick before actors update
void ModuleGameUpdate();
// Called during game cleanup at the end of main()
void ModuleCleanup();
// Called at the end of ShaderUpdateDefaultUniforms(), allowing custom global uniforms to be set
void ModuleExtendShaderDefaultUniforms(Shader shader);
// Called at the end of ShaderUpdateDefaultUniforms(), allowing custom fog uniforms to be set
void ModuleExtendShaderFogUniforms(Shader shader);
// Called at the end of ShaderUpdateLightUniforms(), allowing custom light uniforms to be set
void ModuleExtendShaderLightUniforms(Shader shader);

#endif