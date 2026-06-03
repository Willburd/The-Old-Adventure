#include "tools.h"
#include "assets.h"
#include "actor.h"
#include "camera.h"
#include "input.h"

// private header
void actor_camera_preupdate(struct Actor* actor);
void actor_camera_predrawworld(struct Actor* actor, double tick_percent);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
    int locked;
    int freeaim;
} CameraData;

// Setup the player actor. Public function in the header
void actor_camera_init(struct Actor* actor)
{
    // Configure actor
    actor->func_preupdate = actor_camera_preupdate;
    actor->func_predrawworld = actor_camera_predrawworld;

    // Configure camera
    cam_main = (Camera)
    {
        .position = actor->position,
        .target = VEC3FORWARD,
        .fovy = 90.0f,
        .projection = CAMERA_PERSPECTIVE,
        .up = VEC3UP,
    };
    UpdateCamera(&cam_main, CAMERA_CUSTOM);
    cam_hud = (Camera2D)
    {
        .target = (Vector2) {0,0},
        .offset = (Vector2) {0,0},
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    // Set data
    MALLOC_ACTOR_DATA(CameraData, actor->data);
    CameraData* cam_data = (CameraData*)actor->data;
    cam_data->locked = FALSE;
    cam_data->freeaim = TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void actor_camera_preupdate(struct Actor* actor)
{
    CameraData* cam_data = (CameraData*)actor->data;
    if (cam_data->locked)
        return;
    if (cam_data->freeaim)
    {
        // Rotate actor
        Vector2 input_cam = input_camera;

        // Move actor
        Vector3 axis_move = (Vector3) { input_analog.x, 0, input_analog.y };
        actor->position = Vector3Add(actor->position, Vector3Scale(axis_move, 0.1f));
    }
}

void actor_camera_predrawworld(struct Actor* actor, double tick_percent)
{
    CameraData* cam_data = (CameraData*)actor->data;
    if (cam_data->locked)
        return;
    if (cam_data->freeaim)
    {
        cam_main.position = ACTOR_POS_DELTA(actor, tick_percent);
        cam_main.target = Vector3Add(cam_main.position, Vector3RotateByQuaternion(VEC3FORWARD, ACTOR_ROT_DELTA(actor, tick_percent)));
    }
}
