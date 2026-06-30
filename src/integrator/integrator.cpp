#include "integrator/Integrator.h"
#include "scene/HitRecord.h"
#include <limits>

Integrator::Integrator()
{
}

Vector3 Integrator::trace(const Ray& ray, const Scene& scene, int depth) const
{
    if (depth <= 0)
        return Vector3(0.0f, 0.0f, 0.0f);

    HitRecord hitRecord;
    if (scene.hit(ray, 0.001f, std::numeric_limits<float>::infinity(), hitRecord))
    {
        Vector3 emitted = hitRecord.material->emitted();

        Ray scattered;
        Vector3 attenuation;

        if (hitRecord.material->scatter(ray, hitRecord, attenuation, scattered))
        {
            Vector3 bouncedLight = trace(scattered, scene, depth - 1);
            return emitted + Vector3(
                attenuation.x * bouncedLight.x,
                attenuation.y * bouncedLight.y,
                attenuation.z * bouncedLight.z
            );
        }
        // absorbed or pure emissive — just return emitted (black for non-emissive)
        return emitted;
    }

    return scene.getEnvironment().sample(ray);
}
