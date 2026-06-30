#include "integrator/Integrator.h"
#include "scene/HitRecord.h"
#include "math/Random.h"
#include <limits>
#include <cmath>

Integrator::Integrator()
{
}

Vector3 Integrator::trace(const Ray& ray, const Scene& scene, int depth) const
{
    if (depth <= 0)
        return Vector3(0.0f, 0.0f, 0.0f);

    HitRecord hitRecord;
    if (!scene.hit(ray, 0.001f, std::numeric_limits<float>::infinity(), hitRecord))
        return scene.getEnvironment().sample(ray);

    Vector3 emitted = hitRecord.material->emitted();

    Ray scattered;
    Vector3 attenuation;
    bool didScatter = hitRecord.material->scatter(ray, hitRecord, attenuation, scattered);

    Vector3 directLight(0.0f, 0.0f, 0.0f);

    // ----- Next Event Estimation -----
    const auto& lights = scene.getLights();
    if (didScatter && !lights.empty())
    {
        int lightIndex = static_cast<int>(Random::randomFloat() * lights.size());
        lightIndex = std::min(lightIndex, static_cast<int>(lights.size()) - 1);
        auto& light = lights[lightIndex];

        LightSample ls = light->sample(hitRecord.point);

        if (ls.pdf > 0.0f)
        {
            Vector3 toLight = ls.point - hitRecord.point;
            float distSq = toLight.lengthSquared();
            float dist = std::sqrt(distSq);
            Vector3 lightDir = toLight / dist;

            float cosSurface = hitRecord.normal.dot(lightDir);

            if (cosSurface > 0.0f)
            {
                Ray shadowRay(hitRecord.point, lightDir);
                HitRecord shadowHit;
                bool occluded = scene.hit(shadowRay, 0.001f, dist - 0.001f, shadowHit);

                if (!occluded)
                {
                    float lightPdfArea = ls.pdf / static_cast<float>(lights.size());
                    float brdfPdf = hitRecord.material->scatterPdf(ray, hitRecord, shadowRay);

                    // balance heuristic MIS weight
                    float misWeight = lightPdfArea / (lightPdfArea + brdfPdf + 1e-6f);

                    Vector3 brdfTerm = attenuation * (cosSurface / 3.14159265f);

                    directLight = Vector3(
                        brdfTerm.x * ls.emitted.x,
                        brdfTerm.y * ls.emitted.y,
                        brdfTerm.z * ls.emitted.z
                    ) * (misWeight / lightPdfArea);
                }
            }
        }
    }

    if (didScatter)
    {
        Vector3 bouncedLight = trace(scattered, scene, depth - 1);
        Vector3 indirect = Vector3(
            attenuation.x * bouncedLight.x,
            attenuation.y * bouncedLight.y,
            attenuation.z * bouncedLight.z
        );
        return emitted + directLight + indirect;
    }

    return emitted + directLight;
}