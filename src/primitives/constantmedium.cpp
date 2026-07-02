#include "primitives/ConstantMedium.h"
#include "material/Isotropic.h"
#include "math/Random.h"
#include <limits>
#include <cmath>
#include <algorithm>

ConstantMedium::ConstantMedium(
    std::shared_ptr<Hittable> boundary,
    float density,
    const Vector3& color)
    : boundary(boundary),
      negInvDensity(-1.0f / density),
      phaseFunction(std::make_shared<Isotropic>(color))
{
}

bool ConstantMedium::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
    HitRecord rec1, rec2;

    if (!boundary->hit(ray, -std::numeric_limits<float>::infinity(),
                        std::numeric_limits<float>::infinity(), rec1))
        return false;

    if (!boundary->hit(ray, rec1.t + 0.0001f,
                        std::numeric_limits<float>::infinity(), rec2))
        return false;

    rec1.t = std::max(rec1.t, tMin);
    rec2.t = std::min(rec2.t, tMax);

    if (rec1.t >= rec2.t) return false;
    if (rec1.t < 0.0f) rec1.t = 0.0f;

    float rayLength = ray.getDirection().length();
    float distanceInsideBoundary = (rec2.t - rec1.t) * rayLength;

    float hitDistance = negInvDensity * std::log(Random::randomFloat());

    if (hitDistance > distanceInsideBoundary)
        return false; 

    record.t = rec1.t + hitDistance / rayLength;
    record.point = ray.at(record.t);

    record.normal = Vector3(1.0f, 0.0f, 0.0f);
    record.frontFace = true;
    record.material = phaseFunction;
    record.u = 0.0f;
    record.v = 0.0f;

    return true;
}

AABB ConstantMedium::boundingBox() const
{
    return boundary->boundingBox();
}