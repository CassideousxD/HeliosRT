#ifndef HITRECORD_H
#define HITRECORD_H
#include "math/Vector3.h"
#include "math/Ray.h"
#include "material/Material.h"
#include <memory>

class Hittable;

struct HitRecord
{
    Vector3 point;
    Vector3 normal;
    float t;
    const Hittable* object;
    bool frontFace;
    std::shared_ptr<Material> material;
    void setFaceNormal(const Ray& ray, const Vector3& outwardNormal)
    {
        frontFace = ray.getDirection().dot(outwardNormal) < 0.0f;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
    //UV Mappings
    float u;
    float v;
};

#endif