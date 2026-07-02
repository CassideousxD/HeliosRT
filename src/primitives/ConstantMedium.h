#pragma once
#include "scene/Hittable.h"
#include "material/Material.h"
#include <memory>

class ConstantMedium : public Hittable
{
public:
    ConstantMedium(
        std::shared_ptr<Hittable> boundary,
        float density,
        const Vector3& color);

    bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;
    AABB boundingBox() const override;

private:
    std::shared_ptr<Hittable> boundary;
    float negInvDensity; // -1/density, precomputed
    std::shared_ptr<Material> phaseFunction;
};