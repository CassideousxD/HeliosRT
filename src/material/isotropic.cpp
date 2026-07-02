#include "material/Isotropic.h"
#include "material/SolidColor.h"
#include "scene/HitRecord.h"
#include "math/Random.h"

Isotropic::Isotropic(const Vector3& color)
    : albedo(std::make_shared<SolidColor>(color))
{
}

Isotropic::Isotropic(std::shared_ptr<Texture> texture)
    : albedo(texture)
{
}

bool Isotropic::scatter(
    const Ray& rayIn,
    const HitRecord& hitRecord,
    Vector3& attenuation,
    Ray& scattered
) const
{
    scattered = Ray(hitRecord.point, Random::randomUnitVector(), rayIn.getTime());
    attenuation = albedo->sample(hitRecord.u, hitRecord.v, hitRecord.point);
    return true;
}