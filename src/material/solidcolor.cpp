#include "material/SolidColor.h"

SolidColor::SolidColor(const Vector3& color)
    : color(color)
{
}

Vector3 SolidColor::sample(float u, float v, const Vector3& point) const
{
    return color;
}