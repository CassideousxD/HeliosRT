#include "material/Lambertian.h"
#include "math/Random.h"
#include "scene/HitRecord.h"


Lambertian::Lambertian(const Vector3& color)
    : albedo(std::make_shared<SolidColor>(color))
{
}

Lambertian::Lambertian(std::shared_ptr<Texture> texture)
    : albedo(texture)
{
}

bool Lambertian::scatter(
    const Ray& rayIn,
    const HitRecord& hitRecord,
    Vector3& attenuation,
    Ray& scattered
) const
{
    Vector3 scatterDirection =
        hitRecord.normal + Random::randomUnitVector();

    if (scatterDirection.nearZero())
    {
        scatterDirection = hitRecord.normal;
    }

    scattered = Ray(
        hitRecord.point,
        scatterDirection
    );

    attenuation = albedo->sample(hitRecord.u, hitRecord.v, hitRecord.point);

    return true;
}