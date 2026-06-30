#pragma once
#include "material/Material.h"

class Glass : public Material
{
public:
    explicit Glass(float ior);

    bool scatter(
        const Ray& rayIn,
        const HitRecord& hitRecord,
        Vector3& attenuation,
        Ray& scattered
    ) const override;

private:
    float ior;
    static float schlick(float cosine, float refractiveIndex);
};