#include "tools.h"
#include "assets.h"
#include "actor_factory.h"
#include "camera.h"
#include "input.h"
#include "game_draw.h"
#include "collision.h"

// private header
static void actor_camera_preupdate(struct Actor* actor);
static void actor_camera_drawworld(struct Actor* actor, double tick_percent);
static void actor_camera_postdrawhud(struct Actor* actor, double tick_percent);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
    int locked;
    int camera_mode;
} CameraData;

// Setup the player actor. Public function in the header
void actor_camera_init(struct Actor* actor)
{
    // Configure actor
    actor->actor_flags = ACTOR_FLAG_TICKDURING_GAME | ACTOR_FLAG_TICKDURING_TRANSITION | ACTOR_FLAG_TICKDURING_CUTSCENE;
    actor->func_preupdate = actor_camera_preupdate;
    actor->func_drawworld = actor_camera_drawworld;
    actor->func_postdrawhud = actor_camera_postdrawhud;

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
    cam_data->camera_mode = CAMERA_MODE_FOLLOW;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void UpdateCameraTarget(struct Actor* actor, Vector3 pos)
{
    cam_main.target = pos;
}

static void UpdateCameraPosition(struct Actor* actor, Vector3 pos)
{
    actor->position = pos;
    cam_main.position = pos;
}


static void actor_camera_preupdate(struct Actor* actor)
{
    CameraData* cam_data = (CameraData*)actor->data;
    if (cam_data->locked)
        return;

    struct Actor* player = FINDACTORTYPE(act_player);
    switch (cam_data->camera_mode)
    {
        case CAMERA_MODE_FOLLOW:
        {
            if (player == NULL) // Nothing to look at
                break;
            // Aim camera at player then solve where the camera should be 
            Vector3 look_pos = Vector3Add(player->position, VEC3UP);
            // Raycast from lookpos to the camera and retract if it needs to to avoid being stuck in a wall
            UpdateCameraTarget(actor, look_pos);
            Ray check_ray = {
                .position = look_pos,
                .direction = VEC3DIRECTION(look_pos, cam_main.target)
            };
            RayCollision ray_col = CollisionGetNearest(check_ray, 20.0f, COL_LAYER_WORLD);
            if (ray_col.hit) // Hit a wall, bump out from it!
                UpdateCameraPosition(actor, Vector3Add(ray_col.point, Vector3RotateByQuaternion(Vector3Scale(VEC3FORWARD, 0.01f), VEC3DIRECTIONQUAT(ray_col.point, look_pos))));
        }
        break;

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
            axis_move = Vector3Scale(axis_move, 0.1f);
            UpdateCameraPosition(actor, Vector3Add(actor->position, axis_move));
            // Focus camera ahead
            UpdateCameraTarget(actor, Vector3Add(actor->position, Vector3RotateByQuaternion(VEC3FORWARD, actor->rotation)));
        }
        break;
    }
}

static void actor_camera_drawworld(struct Actor* actor, double tick_percent)
{
    if (!draw_debug_info)
        return;
    // Handle camera lock
    CameraData* cam_data = (CameraData*)actor->data;
    if (cam_data->camera_mode == CAMERA_MODE_FREEMOVE)
        DrawCube(cam_main.target, 0.01f, 0.01f, 0.01f, BLUE);
}

static void actor_camera_postdrawhud(struct Actor* actor, double tick_percent)
{
    if (!draw_debug_info)
        return;
    DrawText(TextFormat("X:%f\nY:%f\nZ:%f", cam_main.target.x, cam_main.target.y, cam_main.target.z), renderWidth / 2, renderHeight / 2, 4, WHITE);
}