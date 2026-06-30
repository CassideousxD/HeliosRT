#include "math/Matrix.h"
#include <cmath>
#include <stdexcept>

Matrix4x4::Matrix4x4()
{
    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            matrix[row][column] = 0.0f;
        }
    }
}

Matrix4x4 Matrix4x4::identity()
{
    Matrix4x4 result;

    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            result.matrix[row][column] = (row == column) ? 1.0f : 0.0f;
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::translation(const Vector3& translation)
{
    Matrix4x4 result = identity();

    result.matrix[0][3] = translation.x;
    result.matrix[1][3] = translation.y;
    result.matrix[2][3] = translation.z;

    return result;
}

Matrix4x4 Matrix4x4::scale(const Vector3& scale)
{
    Matrix4x4 result = identity();

    result.matrix[0][0] = scale.x;
    result.matrix[1][1] = scale.y;
    result.matrix[2][2] = scale.z;

    return result;
}

Matrix4x4 Matrix4x4::rotationX(float angle)
{
    Matrix4x4 result = identity();

    float c = std::cos(angle);
    float s = std::sin(angle);

    result.matrix[1][1] = c;
    result.matrix[1][2] = -s;
    result.matrix[2][1] = s;
    result.matrix[2][2] = c;

    return result;
}

Matrix4x4 Matrix4x4::rotationY(float angle)
{
    Matrix4x4 result = identity();

    float c = std::cos(angle);
    float s = std::sin(angle);

    result.matrix[0][0] = c;
    result.matrix[0][2] = s;
    result.matrix[2][0] = -s;
    result.matrix[2][2] = c;

    return result;
}

Matrix4x4 Matrix4x4::rotationZ(float angle)
{
    Matrix4x4 result = identity();

    float c = std::cos(angle);
    float s = std::sin(angle);

    result.matrix[0][0] = c;
    result.matrix[0][1] = -s;
    result.matrix[1][0] = s;
    result.matrix[1][1] = c;

    return result;
}

Matrix4x4 Matrix4x4::transpose() const
{
    Matrix4x4 result;

    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            result.matrix[row][column] = matrix[column][row];
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
{
    Matrix4x4 result;

    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            result.matrix[row][column] = 0.0f;

            for (int k = 0; k < 4; k++)
            {
                result.matrix[row][column] +=
                    matrix[row][k] * other.matrix[k][column];
            }
        }
    }

    return result;
}

Vector3 Matrix4x4::transformPoint(const Vector3& point) const
{
    float x =
        matrix[0][0] * point.x +
        matrix[0][1] * point.y +
        matrix[0][2] * point.z +
        matrix[0][3];

    float y =
        matrix[1][0] * point.x +
        matrix[1][1] * point.y +
        matrix[1][2] * point.z +
        matrix[1][3];

    float z =
        matrix[2][0] * point.x +
        matrix[2][1] * point.y +
        matrix[2][2] * point.z +
        matrix[2][3];

    return Vector3(x, y, z);
}

Vector3 Matrix4x4::transformDirection(const Vector3& direction) const
{
    float x =
        matrix[0][0] * direction.x +
        matrix[0][1] * direction.y +
        matrix[0][2] * direction.z;

    float y =
        matrix[1][0] * direction.x +
        matrix[1][1] * direction.y +
        matrix[1][2] * direction.z;

    float z =
        matrix[2][0] * direction.x +
        matrix[2][1] * direction.y +
        matrix[2][2] * direction.z;

    return Vector3(x, y, z);
}

float& Matrix4x4::operator()(int row, int column)
{
    return matrix[row][column];
}

float Matrix4x4::operator()(int row, int column) const
{
    return matrix[row][column];
}
