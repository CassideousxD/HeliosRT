#pragma once
#include "material/Texture.h"

class SolidColor : public Texture
{
public:
    explicit SolidColor(const Vector3& color);
    Vector3 sample(float u, float v, const Vector3& point) const override;

private:
    Vector3 color;
};