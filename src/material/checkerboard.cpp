#include "material/Checkerboard.h"
#include <cmath>

Checkerboard::Checkerboard(
    std::shared_ptr<Texture> even,
    std::shared_ptr<Texture> odd,
    float scale)
    : even(even),
      odd(odd),
      scale(scale)
{
}

Vector3 Checkerboard::sample(float u, float v, const Vector3& point) const
{
    // use 3D point so checkerboard is consistent across surfaces
    float sines =
        std::sin(scale * point.x) *
        std::sin(scale * point.y) *
        std::sin(scale * point.z);

    return sines < 0.0f
        ? odd->sample(u, v, point)
        : even->sample(u, v, point);
}