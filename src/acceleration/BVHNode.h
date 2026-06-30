#pragma once
#include "scene/Hittable.h"
#include "acceleration/AABB.h"
#include <vector>
#include <memory>

class BVHNode : public Hittable
{
public:
    BVHNode(std::vector<std::shared_ptr<Hittable>>& objects,
            size_t start, size_t end);

    bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;
    AABB boundingBox() const override;

private:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB box;
};