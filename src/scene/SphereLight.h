#pragma once
#include "scene/Light.h"
#include "math/Vector3.h"

class SphereLight : public Light
{
public:
    SphereLight(const Vector3& center, float radius, const Vector3& emitted);

    LightSample sample(const Vector3& origin) const override;
    float pdf(const Vector3& origin, const Vector3& direction) const override;

private:
    Vector3 center;
    float radius;
    Vector3 emitted;
};