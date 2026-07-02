#pragma once
#include "material/Material.h"
#include "math/Vector3.h"

class RoughDielectric : public Material
{
public:
    RoughDielectric(const Vector3& albedo, float roughness, float ior = 1.5f);

    bool scatter(
        const Ray& rayIn,
        const HitRecord& hitRecord,
        Vector3& attenuation,
        Ray& scattered
    ) const override;

    float scatterPdf(const Ray& rayIn, const HitRecord& hitRecord, const Ray& scattered) const override;

private:
    Vector3 albedo;
    float roughness;
    float ior;
};