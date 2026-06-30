#pragma once
#include "material/Texture.h"
#include <memory>

class Checkerboard : public Texture
{
public:
    Checkerboard(
        std::shared_ptr<Texture> even,
        std::shared_ptr<Texture> odd,
        float scale = 10.0f);

    Vector3 sample(float u, float v, const Vector3& point) const override;

private:
    std::shared_ptr<Texture> even;
    std::shared_ptr<Texture> odd;
    float scale;
};