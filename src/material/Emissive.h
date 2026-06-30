#pragma once
#include "material/Material.h"
#include "math/Vector3.h"

class Emissive : public Material
{
public:
    explicit Emissive(const Vector3& color, float strength = 1.0f);

    Vector3 emitted() const override;

    bool scatter(
        const Ray& rayIn,
        const HitRecord& hitRecord,
        Vector3& attenuation,
        Ray& scattered
    ) const override;

private:
    Vector3 color;
    float strength;
};