#ifndef RAY_H
#define RAY_H
#include "math/Vector3.h"

class Ray
{
public:
    Ray();
    Ray(const Vector3& origin, const Vector3& direction,float time = 0.0f);
    const Vector3& getOrigin() const;
    const Vector3& getDirection() const;
    Vector3 at(float t) const;
    float getTime() const;

private:
    Vector3 origin;
    Vector3 direction;
    float time;
};

#endif