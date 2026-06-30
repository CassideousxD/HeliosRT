#include "scene/SphereLight.h"
#include "math/Random.h"
#include <cmath>
#include <algorithm>

SphereLight::SphereLight(const Vector3& center, float radius, const Vector3& emitted)
    : center(center), radius(radius), emitted(emitted)
{
}

LightSample SphereLight::sample(const Vector3& origin) const
{
    Vector3 dirToCenter = center - origin;
    float distSq = dirToCenter.lengthSquared();
    float dist = std::sqrt(distSq);

    // sample a point on the visible hemisphere of the sphere (cone sampling)
    float cosThetaMax = std::sqrt(std::max(0.0f, 1.0f - (radius*radius)/distSq));
    float r1 = Random::randomFloat();
    float r2 = Random::randomFloat();

    float cosTheta = 1.0f - r1 * (1.0f - cosThetaMax);
    float sinTheta = std::sqrt(std::max(0.0f, 1.0f - cosTheta*cosTheta));
    float phi = 2.0f * 3.14159265f * r2;

    // build local frame around direction to center
    Vector3 w = dirToCenter.normalized();
    Vector3 a = (std::fabs(w.x) > 0.9f) ? Vector3(0,1,0) : Vector3(1,0,0);
    Vector3 u = a.cross(w).normalized();
    Vector3 v = w.cross(u);

    Vector3 sampleDir =
        (u * std::cos(phi) * sinTheta) +
        (v * std::sin(phi) * sinTheta) +
        (w * cosTheta);

    // project sample direction onto sphere surface
    float ds = dist * cosTheta - std::sqrt(std::max(0.0f, radius*radius - dist*dist*sinTheta*sinTheta));
    Vector3 point = origin + sampleDir * ds;
    Vector3 normal = (point - center).normalized();

    float solidAngle = 2.0f * 3.14159265f * (1.0f - cosThetaMax);
    float pdfVal = (solidAngle > 0.0f) ? (1.0f / solidAngle) : 0.0f;

    LightSample result;
    result.point = point;
    result.normal = normal;
    result.emitted = emitted;
    result.pdf = pdfVal;
    return result;
}

float SphereLight::pdf(const Vector3& origin, const Vector3& direction) const
{
    Vector3 dirToCenter = center - origin;
    float distSq = dirToCenter.lengthSquared();
    if (distSq <= radius*radius) return 0.0f;

    float cosThetaMax = std::sqrt(std::max(0.0f, 1.0f - (radius*radius)/distSq));
    float solidAngle = 2.0f * 3.14159265f * (1.0f - cosThetaMax);
    return (solidAngle > 0.0f) ? (1.0f / solidAngle) : 0.0f;
}