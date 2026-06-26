#ifndef __GAME_CAMERA_HEADER__
#define __GAME_CAMERA_HEADER__

#include "raylib.h"

#define CAMERA_MODE_FOLLOW 0
#define CAMERA_MODE_ONLYWATCH 1
#define CAMERA_MODE_FREEMOVE 2
#define CAMERA_MODE_FOCUS_CUTSCENE_SLOW 3
#define CAMERA_MODE_FOCUS_CUTSCENE_FAST 4

typedef struct {
    int locked;
    int camera_mode;
    float follow_angle;
    float pitch_angle;
    Vector3 previous_lookpos;
    Vector3 current_look_pos;
    Vector3 cutscene_look_pos;
} CameraData;

Camera cam_main;
Camera2D cam_hud;

void CameraResetAngleToTarget(struct Actor* camera, float angle);
void SetCutsceneCameraLookPos(struct Actor* camera, Vector3 pos);
void CameraSetMode(struct Actor* camera, int mode);

#endif
