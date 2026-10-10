#include "adv_utility.h"

Quaternion WobbleRotation(Quaternion start_rot, unsigned int wobble_counter, unsigned int wobble_duration, float wobble_angle)
{
    float intensity = 1.0f - ((float)wobble_counter / (float)wobble_duration);
    intensity *= wobble_angle; // angle of wobble
    Quaternion wobble = QuaternionMultiply(start_rot, QuaternionFromAxisAngle(VEC3FORWARD, sinf((float)wobble_counter / 6.0f) * intensity * DEG2RAD));
    return QuaternionMultiply(wobble, QuaternionFromAxisAngle(VEC3RIGHT, cosf((float)wobble_counter / 7.0f) * intensity * DEG2RAD));
}