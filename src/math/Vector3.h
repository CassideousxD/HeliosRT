#ifndef VECTOR3_H
#define VECTOR3_H
#include <cmath>

class Vector3
{
public:
    float x;
    float y;
    float z;

public:
    Vector3();
    Vector3(float x, float y, float z);
    //binary operators
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;
    //compound operators
    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator*=(float scalar);
    Vector3& operator/=(float scalar);
    bool operator==(const Vector3& other) const;
    //unary operators
    Vector3 operator-() const;
    //vector operations
    float lengthSquared() const;
    float length() const;
    Vector3 normalized() const;
    float dot(const Vector3& other) const;
    Vector3 cross(const Vector3& other) const;
    //utility functions
    bool nearZero() const;
    bool operator!=(const Vector3& other) const;
    //Material functions
    Vector3 reflect(const Vector3& normal) const;
    Vector3 refract(const Vector3& normal, float etaRatio) const;
};

#endif