#include "Transform.h"
#include <algorithm>
#include <cmath>

namespace
{
    constexpr float DEG_TO_RAD = 3.14159265358979323846f / 180.0f;
}

Transform::Transform()
    : position(0.0f, 0.0f, 0.0f),
      rotation(0.0f, 0.0f, 0.0f),
      scale(1.0f, 1.0f, 1.0f)
{
    updateMatrices();
}

Transform::Transform(
    const Vector3& position,
    const Vector3& rotation,
    const Vector3& scale)
    : position(position),
      rotation(rotation),
      scale(scale)
{
    updateMatrices();
}

void Transform::setPosition(const Vector3& position)
{
    this->position = position;
    updateMatrices();
}

void Transform::setRotation(const Vector3& rotation)
{
    this->rotation = rotation;
    updateMatrices();
}

void Transform::setScale(const Vector3& scale)
{
    this->scale = scale;
    updateMatrices();
}

const Vector3& Transform::getPosition() const
{
    return position;
}

const Vector3& Transform::getRotation() const
{
    return rotation;
}

const Vector3& Transform::getScale() const
{
    return scale;
}

void Transform::translate(const Vector3& offset)
{
    position += offset;
    updateMatrices();
}

void Transform::rotate(const Vector3& angles)
{
    rotation += angles;
    updateMatrices();
}

void Transform::scaleBy(const Vector3& factor)
{
    scale.x *= factor.x;
    scale.y *= factor.y;
    scale.z *= factor.z;

    updateMatrices();
}

void Transform::updateMatrices()
{
    Matrix4x4 translation =
        Matrix4x4::translation(position);

    Matrix4x4 rotationX =
        Matrix4x4::rotationX(rotation.x * DEG_TO_RAD);

    Matrix4x4 rotationY =
        Matrix4x4::rotationY(rotation.y * DEG_TO_RAD);

    Matrix4x4 rotationZ =
        Matrix4x4::rotationZ(rotation.z * DEG_TO_RAD);

    Matrix4x4 scaling =
        Matrix4x4::scale(scale);

    transformMatrix =
        translation *
        rotationZ *
        rotationY *
        rotationX *
        scaling;

    Matrix4x4 inverseTranslation =
        Matrix4x4::translation(-position);

    Matrix4x4 inverseRotationX =
        Matrix4x4::rotationX(-rotation.x * DEG_TO_RAD);

    Matrix4x4 inverseRotationY =
        Matrix4x4::rotationY(-rotation.y * DEG_TO_RAD);

    Matrix4x4 inverseRotationZ =
        Matrix4x4::rotationZ(-rotation.z * DEG_TO_RAD);

    Matrix4x4 inverseScaling =
        Matrix4x4::scale(
            Vector3(
                1.0f / scale.x,
                1.0f / scale.y,
                1.0f / scale.z));

    inverseTransformMatrix =
        inverseScaling *
        inverseRotationX *
        inverseRotationY *
        inverseRotationZ *
        inverseTranslation;
}

Vector3 Transform::transformPoint(const Vector3& point) const
{
    return transformMatrix.transformPoint(point);
}

Vector3 Transform::transformDirection(const Vector3& direction) const
{
    return transformMatrix.transformDirection(direction);
}

Vector3 Transform::transformNormal(const Vector3& normal) const
{
    return inverseTransformMatrix
        .transpose()
        .transformDirection(normal)
        .normalized();
}

Ray Transform::transformRay(const Ray& ray) const
{
    return Ray(
        transformPoint(ray.getOrigin()),
        transformDirection(ray.getDirection()));
}

Ray Transform::inverseTransformRay(const Ray& ray) const
{
    return Ray(
        inverseTransformMatrix.transformPoint(ray.getOrigin()),
        inverseTransformMatrix.transformDirection(ray.getDirection()));
}

AABB Transform::transformAABB(const AABB& box) const
{
    Vector3 center = (box.minimum + box.maximum) * 0.5f;
    Vector3 extent = (box.maximum - box.minimum) * 0.5f;

    Vector3 transformedCenter =
        transformMatrix.transformPoint(center);

    Matrix4x4 absoluteMatrix = transformMatrix;

    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            absoluteMatrix(row, column) =
                std::abs(absoluteMatrix(row, column));
        }
    }
    Vector3 transformedExtent =
        absoluteMatrix.transformDirection(extent);

    return AABB(
        transformedCenter - transformedExtent,
        transformedCenter + transformedExtent);
}