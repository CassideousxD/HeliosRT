#pragma once
#include "math/Vector3.h"

struct LightSample
{
    Vector3 point;
    Vector3 normal;
    Vector3 emitted;
    float pdf; // solid-angle pdf
};

class Light
{
public:
    virtual ~Light() = default;
    virtual LightSample sample(const Vector3& origin) const = 0;
    virtual float pdf(const Vector3& origin, const Vector3& direction) const = 0;
};