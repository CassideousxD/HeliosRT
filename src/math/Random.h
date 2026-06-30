#pragma once
#include "math/Vector3.h"

class Random
{
public:
    static float randomFloat();
    static float randomFloat(float min, float max);
    static Vector3 randomVector();
    static Vector3 randomVector(float min, float max);
    static Vector3 randomInUnitSphere();
    static Vector3 randomUnitVector();
    static Vector3 randomInHemisphere(const Vector3& normal);
    static Vector3 randomInUnitDisk();
};