#include "TransformNode.h"
#include <algorithm> 

TransformNode::TransformNode(
    std::shared_ptr<Hittable> object,
    const Transform& transform)
    : object(std::move(object)),
      transform(transform),
      transformEnd(transform),
      timeStart(0.0f),
      timeEnd(0.0f),
      isAnimated(false)
{
}

TransformNode::TransformNode(
    std::shared_ptr<Hittable> object,
    const Transform& transformStart,
    const Transform& transformEnd,
    float timeStart,
    float timeEnd)
    : object(std::move(object)),
      transform(transformStart),
      transformEnd(transformEnd),
      timeStart(timeStart),
      timeEnd(timeEnd),
      isAnimated(true)
{
}

bool TransformNode::hit(
    const Ray& ray,
    float tMin,
    float tMax,
    HitRecord& record) const
{
    Transform currentTransform = interpolateTransform(ray.getTime());

    Ray localRay = currentTransform.inverseTransformRay(ray);

    if (!object->hit(localRay, tMin, tMax, record))
    {
        return false;
    }

    record.point = currentTransform.transformPoint(record.point);

    record.normal =
        currentTransform.transformNormal(record.normal).normalized();

    return true;
}

AABB TransformNode::boundingBox() const
{
    AABB localBox = object->boundingBox();

    if (!isAnimated)
        return transform.transformAABB(localBox);

    AABB boxStart = transform.transformAABB(localBox);
    AABB boxEnd   = transformEnd.transformAABB(localBox);

    return boxStart.expand(boxEnd);
}

Transform& TransformNode::getTransform()
{
    return transform;
}

const Transform& TransformNode::getTransform() const
{
    return transform;
}

Transform TransformNode::interpolateTransform(float time) const
{
    if (!isAnimated) return transform;

    float t = (timeEnd - timeStart > 0.0f)
        ? (time - timeStart) / (timeEnd - timeStart)
        : 0.0f;

    t = std::clamp(t, 0.0f, 1.0f);

    Vector3 pos = transform.getPosition()
        + (transformEnd.getPosition() - transform.getPosition()) * t;

    Vector3 rot = transform.getRotation()
        + (transformEnd.getRotation() - transform.getRotation()) * t;

    Vector3 scl = transform.getScale()
        + (transformEnd.getScale() - transform.getScale()) * t;

    return Transform(pos, rot, scl);
}