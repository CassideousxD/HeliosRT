#pragma once
#include "material/Material.h"
#include "math/Vector3.h"

class RoughConductor : public Material
{
public:
    RoughConductor(const Vector3& albedo, float roughness);

    bool scatter(
        const Ray& rayIn,
        const HitRecord& hitRecord,
        Vector3& attenuation,
        Ray& scattered
    ) const override;

    float scatterPdf(const Ray& rayIn, const HitRecord& hitRecord, const Ray& scattered) const override;

private:
    Vector3 albedo;   // used as F0 (tinted metal reflectance)
    float roughness;  // 0 = mirror, 1 = very rough
};