#include "actor.h"
#include "tools.h"
#include <math.h>

int GetFixedRandomInt(Vector3 pos, int offset)
{
    return (int)round((pos.x * 2323.232f) + (pos.y * 3673.245f) + (pos.z * 373.545f) + offset);
}

float GetFixedRandomFloat(Vector3 pos, int offset)
{
    int val = GetFixedRandomInt(pos, offset) * 213553;
    return (float)val / 3422.4212;
}


Vector4 ColorToVector4(Color col)
{
    return (Vector4) { (float)col.r / 255.0f, (float)col.g / 255.0f, (float)col.b / 255.0f, (float)col.a / 255.0f };
}

Vector3 ColorToVector3(Color col)
{
    return (Vector3) { (float)col.r / 255.0f, (float)col.g / 255.0f, (float)col.b / 255.0f };
}

Color Vector4ToColor(Vector4 vec)
{
    return (Color) { (char)(vec.x * (float)255), (char)(vec.y * (float)255), (char)(vec.z * (float)255), (char)(vec.w * (float)255) };
}

Color Vector3ToColor(Vector3 vec, float alpha)
{
    return (Color) { (char)(vec.x * (float)255), (char)(vec.y * (float)255), (char)(vec.z * (float)255), (char)((float)255 * alpha) };
}

float Vector3GetTopDownAngle(Vector3 direction)
{
    return (float)fmod(CIRCLE_DEGREES - (Vector2LineAngle(Vector2Zero(), (Vector2) { -direction.z, direction.x }) * RAD2DEG), CIRCLE_DEGREES) * DEG2RAD;
}

Vector3 Vector3FlatDirection(Vector3 start, Vector3 end)
{
    start.y = 0;
    end.y = 0;
    return VEC3DIRECTION(start, end);
}

float Vector3FlatDistance(Vector3 start, Vector3 end)
{
    start.y = 0;
    end.y = 0;
    return Vector3Distance(start, end);
}

Quaternion QuaternionLookAt(Vector3 start, Vector3 end, Vector3 up)
{
    return QuaternionFromMatrix(MatrixInvert(MatrixLookAt(start, end, up)));
}

Quaternion QuaternionFlatLookAt(Vector3 start, Vector3 end, Vector3 up)
{
    start.y = 0;
    end.y = 0;
    return QuaternionFromMatrix(MatrixInvert(MatrixLookAt(start, end, up)));
}

Quaternion QuaternionGetFlat(Quaternion source, Vector3 up)
{
    Vector3 vec = Vector3RotateByQuaternion(VEC3FORWARD, source);
    return QuaternionFlatLookAt(Vector3Zero(), vec, up);
}

float QuaternionGetTopDownAngle(Quaternion source)
{
    return Vector3GetTopDownAngle(Vector3RotateByQuaternion(VEC3FORWARD, source));
}

#define APPLY_FRICTION(component) \
if (actor->velocity.component > 0.0f) \
{ \
    actor->velocity.component -= amount; \
    if (actor->velocity.component < 0.0f) \
        actor->velocity.component = 0.0f; \
} \
else \
{ \
    actor->velocity.component += amount; \
    if (actor->velocity.component > 0.0f) \
        actor->velocity.component = 0.0f; \
}
void ApplyFriction(struct Actor* actor, float amount)
{
    APPLY_FRICTION(x);
    APPLY_FRICTION(y);
    APPLY_FRICTION(z);
}

void ApplyFlatFriction(struct Actor* actor, float amount)
{
    APPLY_FRICTION(x);
    APPLY_FRICTION(z);
}
#undef APPLY_FRICTION

cJSON* ParseJsonFile(char* path)
{
    // Load material define file
    FILE* fptr = fopen(path, "r");
    if (fptr == NULL)
        return NULL;

    fseek(fptr, 0, SEEK_END);
    uint64_t buffer_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    MALLOC_SIZE(char, buffer_size + 1, buffer, '\0', ERR_NOALLOC);
    size_t read_bytes = fread(buffer, 1, buffer_size, fptr);
    buffer[read_bytes] = '\0';

    cJSON* parsed_data = cJSON_Parse(buffer);
    fclose(fptr);
    free(buffer);

    return parsed_data;
}

void DrawPivotTexture(Texture tex, Vector2 pos, Vector2 pivot, float angle, float scale, Color color)
{
    DrawTextureEx(tex, (Vector2){ 
        pos.x + (sin(angle) * pivot.x) - (cos(angle) * pivot.y),
        pos.y - (sin(angle) * pivot.y) - (cos(angle) * pivot.x)
    }, angle * RAD2DEG, scale, color);
}