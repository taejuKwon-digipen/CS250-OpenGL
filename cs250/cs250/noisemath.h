#pragma once
#include <glm/glm.hpp>

inline float length21(glm::vec3 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline glm::vec3& normalize(glm::vec3 v)
{
    float len2 = length21(v);
    if (len2 > 0) {
        float invLen = 1. / sqrt(len2);
        v.x *= invLen, v.y *= invLen, v.z *= invLen;
    }
    return v;
}

inline float smoothstep2(const float& t)
{
    return t * t * (3 - 2 * t);
}

inline float quintic(const float& t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

inline float quinticDeriv(const float& t)
{
    return 30 * t * t * (t * (t - 2) + 1);
}