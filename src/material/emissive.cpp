#include "material/Emissive.h"

Emissive::Emissive(const Vector3& color, float strength)
    : color(color),
      strength(strength)
{
}

Vector3 Emissive::emitted() const
{
    return color * strength;
}

bool Emissive::scatter(
    const Ray& rayIn,
    const HitRecord& hitRecord,
    Vector3& attenuation,
    Ray& scattered
) const
{
    return false;
}