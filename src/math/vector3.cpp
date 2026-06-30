#include "math/Vector3.h"
#include <cmath>

Vector3::Vector3()
    : x(0.0f),
      y(0.0f),
      z(0.0f)
{
}

Vector3::Vector3(float x, float y, float z)
    : x(x),
      y(y),
      z(z)
{
}

Vector3 Vector3::operator+(const Vector3& other) const
{
    return Vector3(
        x + other.x,
        y + other.y,
        z + other.z
    );
}

Vector3 Vector3::operator-(const Vector3& other) const
{
    return Vector3(
        x - other.x,
        y - other.y,
        z - other.z
    );
}

Vector3 Vector3::operator*(float scalar) const
{
    return Vector3(
        x * scalar,
        y * scalar,
        z * scalar
    );
}

Vector3 Vector3::operator/(float scalar) const
{
    return Vector3(
        x / scalar,
        y / scalar,
        z / scalar
    );
}

Vector3& Vector3::operator+=(const Vector3& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3& Vector3::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3& Vector3::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

bool Vector3::operator==(const Vector3& other) const
{
    return x == other.x &&
           y == other.y &&
           z == other.z;
}

Vector3 Vector3::operator-() const
{
    return Vector3(-x,-y,-z);
}

float Vector3::lengthSquared() const
{
    return x * x +
           y * y +
           z * z;
}

float Vector3::length() const
{
    return std::sqrt(lengthSquared());
}

Vector3 Vector3::normalized() const
{
    float len = length();
    if (len == 0.0f)
    {
        return Vector3();
    }
    return Vector3(
        x / len,
        y / len,
        z / len
    );
}

float Vector3::dot(const Vector3& other) const
{
    return x * other.x +
           y * other.y +
           z * other.z;
}

Vector3 Vector3::cross(const Vector3& other) const
{
    return Vector3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

bool Vector3::nearZero() const
{
    constexpr float epsilon = 1e-8f;
    return std::fabs(x) < epsilon &&
           std::fabs(y) < epsilon &&
           std::fabs(z) < epsilon;
}

bool Vector3::operator!=(const Vector3& other) const
{
    return !(*this == other);
}

Vector3 Vector3::reflect(const Vector3& normal) const
{
    return *this - normal * 2.0f * this->dot(normal);
}

Vector3 Vector3::refract(const Vector3& normal, float etaRatio) const
{
    float cosTheta = std::fmin((-*this).dot(normal), 1.0f);
    Vector3 rOutPerp = (*this + normal * cosTheta) * etaRatio;
    Vector3 rOutParallel =
        normal * -std::sqrt(std::fabs(1.0f - rOutPerp.lengthSquared()));
    return rOutPerp + rOutParallel;
}