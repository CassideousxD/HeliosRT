#pragma once
#include <memory>
#include "Hittable.h"
#include "math/Transform.h"

class TransformNode : public Hittable
{
public:
    TransformNode(
        std::shared_ptr<Hittable> object,
        const Transform& transform);

    TransformNode(
        std::shared_ptr<Hittable> object,
        const Transform& transformStart,
        const Transform& transformEnd,
        float timeStart,
        float timeEnd);

    bool hit(
        const Ray& ray,
        float tMin,
        float tMax,
        HitRecord& record) const override;

    AABB boundingBox() const override;

    Transform& getTransform();
    const Transform& getTransform() const;

private:
    std::shared_ptr<Hittable> object;
    Transform transform;
    Transform transformEnd;
    float timeStart;
    float timeEnd;
    bool isAnimated;
    Transform interpolateTransform(float time) const;
};