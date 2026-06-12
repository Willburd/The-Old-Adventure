#ifndef __GAME_CAMERA_HEADER__
#define __GAME_CAMERA_HEADER__

#include "raylib.h"

#define CAMERA_MODE_FOLLOW 0
#define CAMERA_MODE_FREEMOVE 1

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

#endif
