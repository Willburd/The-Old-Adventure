#ifndef __GAME_CAMERA_HEADER__
#define __GAME_CAMERA_HEADER__

#include "raylib.h"

#define CAMERA_MODE_FREEMOVE 0

typedef struct {
    int locked;
    int camera_mode;
    float follow_angle;
    float pitch_angle;
    Vector3 previous_lookpos;
    Vector3 current_look_pos;
    Vector3 forced_look_pos;
} CameraData;

Camera cam_main;
Camera2D cam_hud;

void CameraResetAngleToTarget(struct Actor* camera, float angle);
void CameraSetForcedLookPos(struct Actor* camera, Vector3 pos);
void CameraSetMode(struct Actor* camera, int mode);

#endif
