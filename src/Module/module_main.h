#ifndef __MODULE_MAIN_HEADER__
#define __MODULE_MAIN_HEADER__

// Called during game's initilization in main()
void ModuleInit();
// Called during each update loop before checking if the gameupdate tick should happen
void ModuleInternalUpdate();
// Called during each gametick before actors update
void ModuleGameUpdate();
// Called during game cleanup at the end of main()
void ModuleCleanup();

#endif