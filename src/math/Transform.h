#pragma once
#include "Matrix.h"
#include "Ray.h"
#include "Vector3.h"
#include "acceleration/AABB.h"

class Transform
{
public:
    Transform();

    Transform(
        const Vector3& position,
        const Vector3& rotation,
        const Vector3& scale);

    // Setters
    void setPosition(const Vector3& position);
    void setRotation(const Vector3& rotation);
    void setScale(const Vector3& scale);

    // Getters
    const Vector3& getPosition() const;
    const Vector3& getRotation() const;
    const Vector3& getScale() const;

    // Relative transforms
    void translate(const Vector3& offset);
    void rotate(const Vector3& angles);
    void scaleBy(const Vector3& factor);

    // Transform helpers
    Vector3 transformPoint(const Vector3& point) const;
    Vector3 transformDirection(const Vector3& direction) const;
    Vector3 transformNormal(const Vector3& normal) const;

    Ray transformRay(const Ray& ray) const;
    Ray inverseTransformRay(const Ray& ray) const;
    AABB transformAABB(const AABB& box) const;

private:
    void updateMatrices();
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

    Matrix4x4 transformMatrix;
    Matrix4x4 inverseTransformMatrix;
};