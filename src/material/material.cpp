#include "material/Material.h"
#include "scene/HitRecord.h"
#include <cmath>

float Material::scatterPdf(const Ray& rayIn, const HitRecord& hitRecord, const Ray& scattered) const
{
    float cosine = hitRecord.normal.dot(scattered.getDirection().normalized());
    return cosine > 0.0f ? cosine / 3.14159265f : 0.0f;
}