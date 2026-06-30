#pragma once
#include <array>
#include "Vector3.h"

class Matrix4x4
{
public:
    Matrix4x4();
    static Matrix4x4 identity();
    static Matrix4x4 translation(const Vector3& translation);
    static Matrix4x4 scale(const Vector3& scale);
    static Matrix4x4 rotationX(float angle);
    static Matrix4x4 rotationY(float angle);
    static Matrix4x4 rotationZ(float angle);
    Matrix4x4 transpose() const;
    Vector3 transformPoint(const Vector3& point) const;
    Vector3 transformDirection(const Vector3& direction) const;
    Matrix4x4 operator*(const Matrix4x4& other) const;
    float& operator()(int row, int column);
    float operator()(int row, int column) const;

private:
    std::array<std::array<float, 4>, 4> matrix;
};