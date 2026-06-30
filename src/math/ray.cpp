#include "math/Ray.h"

Ray::Ray()
    : origin(),
      direction(),
      time(0.0f)
{
}

Ray::Ray(const Vector3& origin, const Vector3& direction, float time)
    : origin(origin),
      direction(direction),
      time(time)
{
}

float Ray::getTime() const
{
    return time;
}

const Vector3& Ray::getOrigin() const
{
    return origin;
}

const Vector3& Ray::getDirection() const
{
    return direction;
}

Vector3 Ray::at(float t) const
{
    return origin + direction * t;
}
