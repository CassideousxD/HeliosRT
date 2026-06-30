#pragma once
#include "math/Vector3.h"

class Texture
{
public:
    virtual ~Texture() = default;
    virtual Vector3 sample(float u, float v, const Vector3& point) const = 0;
};