#ifndef VMATH
#define VMATH

#include <Arduino.h>

float length(float v[3])
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float dotProduct(float v1[3], float v2[3])
{
    return v1[0] * v2[0] +
           v1[1] * v2[1] +
           v1[2] * v2[2];
}

float* multiply(float v[3], float scalar)
{
    float result[3] = {
        v[0] * scalar,
        v[1] * scalar,
        v[2] * scalar
    };

    return result;
}

float* add(float v1[3], float v2[3])
{
    float result[3] = {
        v1[0] + v2[0],
        v1[1] + v2[1],
        v1[2] + v2[2]
    };

    return result;
}

float* divide(float v[3], float scalar)
{
    float result[3] = {
        v[0] / scalar,
        v[1] / scalar,
        v[2] / scalar
    };

    return result;
}

#endif