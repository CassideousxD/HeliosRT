#pragma once
#include "scene/Hittable.h"
#include "acceleration/BVHNode.h"
#include <vector>
#include <memory>

class Mesh : public Hittable
{
public:
    Mesh(std::vector<std::shared_ptr<Hittable>> triangles);

    bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;
    AABB boundingBox() const override;

private:
    std::vector<std::shared_ptr<Hittable>> triangles;
    std::shared_ptr<BVHNode> bvh;
    AABB box;
};