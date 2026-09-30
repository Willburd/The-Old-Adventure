#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "camera.h"
#include "input.h"
#include "game_draw.h"
#include "collision.h"

// private header
ACTOR_PREUPDATE(camera);
ACTOR_PREDRAWWORLD(camera);
ACTOR_DRAWWORLD(camera);
static void UpdateCameraTargetPosition(struct Actor* camera, Vector3 target_pos);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(camera)
{
    // Configure actor
    actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TEXTBOX | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_TICKDURING_CUTSCENE | ACTOR_FLAG_TICKDURING_PAUSED;
    ACTOR_REGISTER_PREUPDATE(camera);
    ACTOR_REGISTER_PREDRAWWORLD(camera);
    ACTOR_REGISTER_DRAWWORLD(camera);

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
    cam_data->camera_mode = CAMERA_MODE_FREEMOVE;
    cam_data->follow_angle = 0.0f;
    cam_data->pitch_angle = 0.0f;
}

void CameraResetAngleToTarget(struct Actor* camera, float angle)
{
    CameraData* cam_data = (CameraData*)camera->data;
    cam_data->follow_angle = angle;
    cam_data->pitch_angle = -3.0f;
    printf("Recalculated camera angle: %f\n", cam_data->follow_angle);
}

void CameraSetForcedLookPos(struct Actor* camera, Vector3 pos)
{
    CameraData* cam_data = (CameraData*)camera->data;
    cam_data->forced_look_pos = pos;
}

void CameraSetMode(struct Actor* camera, int mode)
{
    CameraData* cam_data = (CameraData*)camera->data;
    cam_data->camera_mode = mode;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_PREUPDATE(camera)
{
    // Update camera delta target
    CameraData* cam_data = (CameraData*)actor->data;
    cam_data->previous_lookpos = cam_data->current_look_pos;

    if (cam_data->locked)
        return;

    // Update camera logic
    switch (cam_data->camera_mode)
    {
        case CAMERA_MODE_FREEMOVE:
        {
            // Identity vectors
            Vector3 right = Vector3RotateByQuaternion(VEC3RIGHT, actor->rotation);
            // Rotate actor
            Quaternion cam_rot = QuaternionFromAxisAngle(VEC3UP, input_camera.x); // Stationary vertical axis
            cam_rot = QuaternionMultiply(cam_rot, QuaternionFromAxisAngle(right, input_camera.y)); // Relative horizontal axis to camera
            actor->rotation = QuaternionNormalize(QuaternionMultiply(cam_rot, actor->rotation)); // Apply and normalize
            // Move actor
            Vector3 axis_move = { -input_analog.x, 0, input_analog.y }; // TODO - Find out why I need to invert the x axis.
            axis_move = Vector3RotateByQuaternion(axis_move, actor->rotation);
            axis_move = Vector3Scale(axis_move, 0.4f);
            actor->position = Vector3Add(actor->position, axis_move);
            // Focus camera ahead
            UpdateCameraTargetPosition(actor, Vector3Add(actor->position, Vector3RotateByQuaternion(VEC3FORWARD, actor->rotation)));
        }
        break;
    }
}

ACTOR_PREDRAWWORLD(camera)
{
    CameraData* cam_data = (CameraData*)actor->data;
    cam_main.target = Vector3Lerp(cam_data->previous_lookpos, cam_data->current_look_pos, (float)tick_percent);
    cam_main.position = ACTOR_POS_DELTA(actor, tick_percent);
}

ACTOR_DRAWWORLD(camera)
{
    if (!draw_debug_info)
        return;
    // Handle camera lock
    CameraData* cam_data = (CameraData*)actor->data;
    switch (cam_data->camera_mode)
    {
        case CAMERA_MODE_FREEMOVE:
        {
            DrawCube(cam_main.target, 0.01f, 0.01f, 0.01f, BLUE);

            // Raycast from lookpos to the camera and retract if it needs to to avoid being stuck in a wall
            Ray check_ray = {
                .position = cam_main.position,
                .direction = VEC3DIRECTION(cam_main.position, cam_main.target)
            };
            RayHitData ray_col = CollisionGetNearest(check_ray, 10.0f, COL_LAYER_CAMERA);
            if (ray_col.ray_col.hit) // Hit a wall, bump out from it!
            {
                DrawSphere(ray_col.ray_col.point, 0.06f, YELLOW);
            }
        }
        break;
    }
}

static void UpdateCameraTargetPosition(struct Actor* camera, Vector3 target_pos)
{
    CameraData* cam_data = (CameraData*)camera->data;
    cam_data->current_look_pos = target_pos;
}

