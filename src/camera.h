#ifndef __GAME_CAMERA_HEADER__
#define __GAME_CAMERA_HEADER__

#include "raylib.h"

#define CAMERA_MODE_FOLLOW 0
#define CAMERA_MODE_ONLYWATCH 1
#define CAMERA_MODE_FREEMOVE 2

typedef struct {
    int locked;
    int camera_mode;
    float follow_angle;
    float pitch_angle;
    Vector3 previous_lookpos;
    Vector3 current_look_pos;
} CameraData;

Camera cam_main;
Camera2D cam_hud;

void CameraRecalculateAngleToTarget(struct Actor* camera);
Matrix GetActorCameraMatrix();
Matrix GetActorCameraMatrixInverseRotation();
Transform GetActorCameraTransform();

#endif
