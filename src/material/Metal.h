#pragma once
#include "material/Material.h"
#include "math/Vector3.h"

class Metal : public Material
{
public:
    Metal(const Vector3& albedo, float fuzz);

    bool scatter(
        const Ray& rayIn,
        const HitRecord& hitRecord,
        Vector3& attenuation,
        Ray& scattered
    ) const override;

private:
    Vector3 albedo;
    float fuzz;
};