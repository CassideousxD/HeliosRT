#pragma once
#include "math/Vector3.h"
#include "math/Ray.h"
#include <algorithm>

class AABB
{
public:
    //Axis-Aligned Bouding Box
    AABB() = default;
    AABB(const Vector3& minimum, const Vector3& maximum);

    bool hit(const Ray& ray, float tMin, float tMax) const;

    // expand this box to also contain other box
    AABB expand(const AABB& other) const;

    Vector3 minimum;
    Vector3 maximum;
};