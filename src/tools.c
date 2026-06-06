#include "tools.h"

inline int clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

inline float fclamp(float val, float min, float max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}