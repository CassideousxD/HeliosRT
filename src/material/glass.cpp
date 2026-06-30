#include "material/Glass.h"
#include "scene/HitRecord.h"
#include "math/Random.h"
#include <cmath>

Glass::Glass(float ior)
    : ior(ior)
{
}

float Glass::schlick(float cosine, float refractiveIndex)
{
    float r0 = (1.0f - refractiveIndex) / (1.0f + refractiveIndex);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * std::pow(1.0f - cosine, 5.0f);
}

bool Glass::scatter(
    const Ray& rayIn,
    const HitRecord& hitRecord,
    Vector3& attenuation,
    Ray& scattered
) const
{
    // glass doesn't tint light
    attenuation = Vector3(1.0f, 1.0f, 1.0f);

    float etaRatio = hitRecord.frontFace ? (1.0f / ior) : ior;
    Vector3 unitDir = rayIn.getDirection().normalized();
    float cosTheta = std::fmin((-unitDir).dot(hitRecord.normal), 1.0f);
    float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);
    bool cannotRefract = etaRatio * sinTheta > 1.0f;

    Vector3 direction;

    if (cannotRefract || schlick(cosTheta, etaRatio) > Random::randomFloat())
    {
        // total internal reflection or Fresnel reflection
        direction = unitDir.reflect(hitRecord.normal);
    }
    else
    {
        direction = unitDir.refract(hitRecord.normal, etaRatio);
    }

    scattered = Ray(hitRecord.point, direction);
    return true;
}