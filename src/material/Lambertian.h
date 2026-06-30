#pragma once
#include "material/Material.h"
#include "material/Texture.h"
#include "material/SolidColor.h"
#include <memory>

class Lambertian : public Material
{
public:
    bool scatter(
        const Ray& rayIn,
        const HitRecord& hitRecord,
        Vector3& attenuation,
        Ray& scattered
    ) const override;
    explicit Lambertian(const Vector3& color);
    explicit Lambertian(std::shared_ptr<Texture> texture);

private:
    std::shared_ptr<Texture> albedo;
};