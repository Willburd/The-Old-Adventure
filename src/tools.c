#include "tools.h"

Vector4 ColorToVector4(Color col)
{
    return (Vector4) { (float)col.r / 255.0f, (float)col.g / 255.0f, (float)col.b / 255.0f, (float)col.a / 255.0f };
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
    return Vector2Angle((Vector2){ 0.0f, -1.0f }, (Vector2) { direction.x, direction.z });
}