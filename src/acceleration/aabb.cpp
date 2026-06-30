#include "acceleration/AABB.h"
#include <cmath>

AABB::AABB(const Vector3& minimum, const Vector3& maximum)
    : minimum(minimum),
      maximum(maximum)
{
}

bool AABB::hit(const Ray& ray, float tMin, float tMax) const
{
    const Vector3& orig = ray.getOrigin();
    const Vector3& dir  = ray.getDirection();

    for (int axis = 0; axis < 3; axis++)
    {
        float origAxis = (axis == 0) ? orig.x : (axis == 1) ? orig.y : orig.z;
        float dirAxis  = (axis == 0) ? dir.x  : (axis == 1) ? dir.y  : dir.z;
        float minAxis  = (axis == 0) ? minimum.x : (axis == 1) ? minimum.y : minimum.z;
        float maxAxis  = (axis == 0) ? maximum.x : (axis == 1) ? maximum.y : maximum.z;

        float invD = 1.0f / dirAxis;
        float t0 = (minAxis - origAxis) * invD;
        float t1 = (maxAxis - origAxis) * invD;

        if (invD < 0.0f) std::swap(t0, t1);

        tMin = std::max(t0, tMin);
        tMax = std::min(t1, tMax);

        if (tMax <= tMin) return false;
    }
    return true;
}

AABB AABB::expand(const AABB& other) const
{
    Vector3 newMin(
        std::min(minimum.x, other.minimum.x),
        std::min(minimum.y, other.minimum.y),
        std::min(minimum.z, other.minimum.z)
    );
    Vector3 newMax(
        std::max(maximum.x, other.maximum.x),
        std::max(maximum.y, other.maximum.y),
        std::max(maximum.z, other.maximum.z)
    );
    return AABB(newMin, newMax);
}