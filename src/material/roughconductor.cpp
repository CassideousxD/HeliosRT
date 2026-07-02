#include "material/RoughConductor.h"
#include "material/GGX.h"
#include "scene/HitRecord.h"
#include "math/Random.h"

RoughConductor::RoughConductor(const Vector3& albedo, float roughness)
    : albedo(albedo),
      roughness(std::clamp(roughness, 0.02f, 1.0f)) // avoid alpha=0 singularities
{
}

bool RoughConductor::scatter(
    const Ray& rayIn,
    const HitRecord& hitRecord,
    Vector3& attenuation,
    Ray& scattered
) const
{
    float alpha = roughness * roughness;
    Vector3 n = hitRecord.normal;
    Vector3 v = -rayIn.getDirection().normalized();

    float u1 = Random::randomFloat();
    float u2 = Random::randomFloat();

    Vector3 h = GGX::sampleHalfVector(n, alpha, u1, u2);
    Vector3 l = (h * 2.0f * v.dot(h) - v).normalized(); // reflect v about h

    float nDotL = n.dot(l);
    float nDotV = n.dot(v);
    if (nDotL <= 0.0f || nDotV <= 0.0f) return false;

    float nDotH = std::max(n.dot(h), 0.0f);
    float vDotH = std::max(v.dot(h), 0.0f);

    float D = GGX::distribution(nDotH, alpha);
    float G = GGX::geometry(nDotV, nDotL, alpha);
    Vector3 F = GGX::fresnelSchlick(vDotH, albedo);

    Vector3 specular = F * (D * G / std::max(4.0f * nDotV * nDotL, 1e-8f));

    float pdf = GGX::pdfHalfVector(nDotH, vDotH, alpha);
    if (pdf <= 0.0f) return false;

    // attenuation = BRDF * cos(theta) / pdf  (Monte Carlo estimator)
    attenuation = specular * (nDotL / pdf);

    scattered = Ray(hitRecord.point, l, rayIn.getTime());
    return true;
}

float RoughConductor::scatterPdf(const Ray& rayIn, const HitRecord& hitRecord, const Ray& scattered) const
{
    float alpha = roughness * roughness;
    Vector3 n = hitRecord.normal;
    Vector3 v = -rayIn.getDirection().normalized();
    Vector3 l = scattered.getDirection().normalized();
    Vector3 h = (v + l).normalized();

    float nDotH = std::max(n.dot(h), 0.0f);
    float vDotH = std::max(v.dot(h), 0.0f);

    return GGX::pdfHalfVector(nDotH, vDotH, alpha);
}