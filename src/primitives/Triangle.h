#pragma once
#include "scene/Hittable.h"
#include "math/Vector3.h"
#include <memory>

class Material;

struct TriangleVertex
{
    Vector3 position;
    Vector3 normal;
    float u = 0.0f;
    float v = 0.0f;
};

class Triangle : public Hittable
{
public:
    Triangle(
        const TriangleVertex& v0,
        const TriangleVertex& v1,
        const TriangleVertex& v2,
        std::shared_ptr<Material> material);

    bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;
    AABB boundingBox() const override;

private:
    TriangleVertex v0, v1, v2;
    Vector3 faceNormal;
    std::shared_ptr<Material> material;
};