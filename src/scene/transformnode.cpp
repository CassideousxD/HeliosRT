#include "TransformNode.h"

TransformNode::TransformNode(
    std::shared_ptr<Hittable> object,
    const Transform& transform)
    : object(std::move(object)),
      transform(transform)
{
}

bool TransformNode::hit(
    const Ray& ray,
    float tMin,
    float tMax,
    HitRecord& record) const
{
    Ray localRay = transform.inverseTransformRay(ray);

    if (!object->hit(localRay, tMin, tMax, record))
    {
        return false;
    }

    record.point = transform.transformPoint(record.point);

    record.normal =
        transform.transformNormal(record.normal).normalized();

    return true;
}

AABB TransformNode::boundingBox() const
{
    AABB box = object->boundingBox();
    return transform.transformAABB(box);
}

Transform& TransformNode::getTransform()
{
    return transform;
}

const Transform& TransformNode::getTransform() const
{
    return transform;
}