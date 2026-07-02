#pragma once
#include "material/Material.h"
#include "math/Vector3.h"
#include <memory>

class Texture;

class Isotropic : public Material
{
public:
    explicit Isotropic(const Vector3& color);
    explicit Isotropic(std::shared_ptr<Texture> texture);

    bool scatter(
        const Ray& rayIn,
        const HitRecord& hitRecord,
        Vector3& attenuation,
        Ray& scattered
    ) const override;

private:
    std::shared_ptr<Texture> albedo;
};