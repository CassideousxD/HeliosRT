#pragma once

#include "math/Ray.h"
#include "math/Vector3.h"

class HitRecord;

class Material
{
public:
    Material() = default;
    virtual ~Material() = default;
    virtual bool scatter(
        const Ray& rayIn,
        const HitRecord& hitRecord,
        Vector3& attenuation,
        Ray& scattered
    ) const = 0;
    virtual Vector3 emitted() const
    {
        return Vector3(0.0f, 0.0f, 0.0f);
    }
};