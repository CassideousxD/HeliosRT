#include "material/Metal.h"
#include "scene/HitRecord.h"
#include "math/Random.h"

Metal::Metal(const Vector3& albedo, float fuzz)
    : albedo(albedo),
      fuzz(fuzz < 1.0f ? fuzz : 1.0f)
{
}

bool Metal::scatter(
    const Ray& rayIn,
    const HitRecord& hitRecord,
    Vector3& attenuation,
    Ray& scattered
) const
{
    Vector3 reflected =
        rayIn.getDirection().reflect(hitRecord.normal);

    // normalize then add fuzz so fuzz scale is consistent
    reflected = reflected.normalized()
              + Random::randomUnitVector() * fuzz;

    // if fuzzed ray points into surface — absorb it
    if (reflected.dot(hitRecord.normal) <= 0.0f)
        return false;

    scattered = Ray(hitRecord.point, reflected);
    attenuation = albedo;
    return true;
}