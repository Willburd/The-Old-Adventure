#include "tools.h"

Vector4 ColorToVector4(Color col)
{
    return (Vector4) { (float)col.r / 255.0f, (float)col.g / 255.0f, (float)col.b / 255.0f, (float)col.a / 255.0f };
}

Color Vector4ToColor(Vector4 vec)
{
    return (Color) { vec.x * 255, vec.y * 255, vec.z * 255, vec.w * 255 };
}