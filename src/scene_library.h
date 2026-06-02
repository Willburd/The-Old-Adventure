#ifndef SCENE_LIBRARY_HEADER
#define SCENE_LIBRARY_HEADER

typedef enum
{
	debugscene,
	bootscene,
	titlescene,
	testscene,
	LAST_SCENE

} SceneID;

// All game scenes
void scene_debugscene_init(struct Actor* scene);
void scene_titlescene_init(struct Actor* scene);
void scene_testscene_init(struct Actor* scene);

#endif