#include "material/RoughDielectric.h"
#include "material/GGX.h"
#include "scene/HitRecord.h"
#include "math/Random.h"

RoughDielectric::RoughDielectric(const Vector3& albedo, float roughness, float ior)
    : albedo(albedo),
      roughness(std::clamp(roughness, 0.02f, 1.0f)),
      ior(ior)
{
}

bool RoughDielectric::scatter(
    const Ray& rayIn,
    const HitRecord& hitRecord,
    Vector3& attenuation,
    Ray& scattered
) const
{
    Vector3 n = hitRecord.normal;
    Vector3 v = -rayIn.getDirection().normalized();
    float nDotV = std::max(n.dot(v), 1e-4f);

    float f0Scalar = (1.0f - ior) / (1.0f + ior);
    f0Scalar *= f0Scalar;
    Vector3 f0(f0Scalar, f0Scalar, f0Scalar);

    Vector3 F = GGX::fresnelSchlick(nDotV, f0);
    float specProb = std::clamp((F.x + F.y + F.z) / 3.0f, 0.05f, 0.95f);

    if (Random::randomFloat() < specProb)
    {
        // ---- specular GGX lobe ----
        float alpha = roughness * roughness;
        float u1 = Random::randomFloat();
        float u2 = Random::randomFloat();

        Vector3 h = GGX::sampleHalfVector(n, alpha, u1, u2);
        Vector3 l = (h * 2.0f * v.dot(h) - v).normalized();

        float nDotL = n.dot(l);
        if (nDotL <= 0.0f) return false;

        float nDotH = std::max(n.dot(h), 0.0f);
        float vDotH = std::max(v.dot(h), 0.0f);

        float D = GGX::distribution(nDotH, alpha);
        float G = GGX::geometry(nDotV, nDotL, alpha);
        Vector3 Fh = GGX::fresnelSchlick(vDotH, f0);

        Vector3 specular = Fh * (D * G / std::max(4.0f * nDotV * nDotL, 1e-8f));
        float pdf = GGX::pdfHalfVector(nDotH, vDotH, alpha) * specProb;
        if (pdf <= 0.0f) return false;

        attenuation = specular * (nDotL / pdf);
        scattered = Ray(hitRecord.point, l, rayIn.getTime());
        return true;
    }
    else
    {
        // ---- diffuse lobe (Lambertian) ----
        Vector3 scatterDir = n + Random::randomUnitVector();
        if (scatterDir.nearZero()) scatterDir = n;

        scattered = Ray(hitRecord.point, scatterDir, rayIn.getTime());

        // energy not reflected specularly goes to diffuse, scaled by (1-F)
        Vector3 kd = Vector3(1,1,1) - F;
        attenuation = Vector3(
            albedo.x * kd.x,
            albedo.y * kd.y,
            albedo.z * kd.z
        ) / (1.0f - specProb);

        return true;
    }
}

float RoughDielectric::scatterPdf(const Ray& rayIn, const HitRecord& hitRecord, const Ray& scattered) const
{
    // approximate combined pdf as cosine-weighted (good enough for MIS with NEE)
    float cosine = hitRecord.normal.dot(scattered.getDirection().normalized());
    return cosine > 0.0f ? cosine / 3.14159265f : 0.0f;
}